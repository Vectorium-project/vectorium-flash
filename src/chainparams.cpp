// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "key_io.h"
#include "main.h"
#include "crypto/equihash.h"

#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, const uint256& nNonce, const std::vector<unsigned char>& nSolution, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    // To create a genesis block for a new chain which is Overwintered:
    //   txNew.nVersion = OVERWINTER_TX_VERSION
    //   txNew.fOverwintered = true
    //   txNew.nVersionGroupId = OVERWINTER_VERSION_GROUP_ID
    //   txNew.nExpiryHeight = <default value>
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 537534368 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nSolution = nSolution;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = genesis.BuildMerkleTree();
    return genesis;
}

 /**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database (and is in any case of vectorium value).
 *
 * >>> from pyblake2 import blake2s
 * >>> 'VECT' + blake2s(b'2019 - Vectorium - In CryptoEnergy we trust').hexdigest()
 */
static CBlock CreateGenesisBlock(uint32_t nTime, const uint256& nNonce, const std::vector<unsigned char>& nSolution, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "VECT1dd972e914e8f9196a825fd2996110049506a5c8abe65297e36bd6444b9ca1fa";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nSolution, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

const arith_uint256 maxUint = UintToArith256(uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        strCurrencyUnits = "VECT";
        // bip44CoinType = 133; // 133 is Zcash registered in https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        consensus.fCoinbaseMustBeProtected = false;
        consensus.nSubsidySlowStartInterval = 0;
        consensus.nSubsidyHalvingInterval = 0;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 4000;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 2;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 30; // 30% adjustment down
        consensus.nPowMaxAdjustUp = 10; // 10% adjustment up
        consensus.nPowTargetSpacing = 30;
        consensus.nPowAllowMinDifficultyBlocksAfterHeight = boost::none;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nActivationHeight =
            Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nProtocolVersion = 170005;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nActivationHeight = 1000;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nProtocolVersion = 170007;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nActivationHeight = 1000;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000000000d9c44c94");

        /**
         * The message start string should be awesome! ⓩ❤
         */
        pchMessageStart[0] = 0x56; // V
        pchMessageStart[1] = 0x45; // E
        pchMessageStart[2] = 0x43; // C
        pchMessageStart[3] = 0x54; // T
        vAlertPubKey = ParseHex("73B0");
        nDefaultPort = 25801;
        nPruneAfterHeight = 100000;
        const size_t N = 192, K = 7;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;

        genesis = CreateGenesisBlock(
            1548201600,
            uint256S("000000000000000000000000000000000000000000000000000000000000001c"),
            ParseHex("0033868c02bd22b7d5d62c3ee0723e3658487cfbee60ba5abb137fbfb88f12f69abc5c80b33518b44d26fb3b0f3b8db7b3eb12cf26d372a7d6aae17e27f869aff30793652799690bdbef3027ebbdce8719a8f6b3fcd7e9f6be544630262a45d39ffd4a2d020802ba9b70cb93178c3b5a83d586ba46ce6b693a91fab795234a8dfb34060d0dc7f136d215a88e766aaff6e6a29bfa97e30591f7af99dfe1b7d294a17fe17c77ee174ab88fc2530e1fff2497b29bec065a1f8d52236d12ad48ca7b675a86047195eb2f00ee8fce999be6f7f37baa826156f9aa6a600db499e702455f3c7ceaa135be56848f10d59bc45eca5414b8560d9fbb75adca09d33b192538b4b96b3ddc09810f5658e3961d115074d679fb0ca19a4d009561211053e292c2cc90c1f4fbacb52339a181fc08cf581a173388f9cab72b12075dc9be56ae120a645d43447727e9d8b77b5f63e6d457845ed3869ee73a2349c8abad4bfd990efbf7415481078f84312e7c821443ca86f6dcf55fd289b500279bbe3ec2ab2f180ffa1d36844e3e1710b1816d35bfc18aea"),
            0x200A1FA<<4, 4, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
                 
        assert(consensus.hashGenesisBlock == uint256S("0x09f30d3dcb2471db0a9017d3788340004b9ac6de74afa2f9231df388204ca114"));
        assert(genesis.hashMerkleRoot == uint256S("0x5ed9fac3a1b679ad90cb6dc1cc51de19d7ea811fc2465713ab67eb4bf0c1d321"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("vectorium.co", "seed1.vectorium.co")); //Vectorium seed node
		vSeeds.push_back(CDNSSeedData("vectorium.co", "seed2.vectorium.co")); //Vectorium seed node
		vSeeds.push_back(CDNSSeedData("vectorium.co", "seed3.vectorium.co")); //Vectorium seed node
		vSeeds.push_back(CDNSSeedData("vectorium.co", "seed4.vectorium.co")); //Vectorium seed node
		vSeeds.push_back(CDNSSeedData("vectorium.co", "seed5.vectorium.co")); //Vectorium seed node

        // guarantees the first 2 characters, when base58 encoded, are "V1"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x0F,0xC5};
        // guarantees the first 2 characters, when base58 encoded, are "t3"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x1C,0xBD};
        // the first character, when base58 encoded, is "5" or "K" or "L" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0x80};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x88,0xB2,0x1E};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x88,0xAD,0xE4};
        // guarantees the first 2 characters, when base58 encoded, are "zc"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0x9A};
        // guarantees the first 4 characters, when base58 encoded, are "ZiVK"
        base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAB,0xD3};
        // guarantees the first 2 characters, when base58 encoded, are "SK"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAB,0x36};

        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "zs";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "zviews";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "zivks";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "secret-extended-key-main";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        
        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (0, consensus.hashGenesisBlock)
            (150000, uint256S("0x00006fcaba09059d8affe7ebd5e4403fc52a1c66fb9307e495c7a915436d0f34")),
            1555974824, // * UNIX timestamp of last checkpoint block
            245722, // * total number of transactions between genesis and last checkpoint
            2900 // * estimated number of transactions per day after checkpoint
        };

        // Founders reward script expects a vector of 2-of-3 multisig addresses
        vFoundersRewardAddress = {};
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());
        
		// Licensed miners
		consensus.nMinerLicenseActivationHeight = 150000;
		vLicensedMiners = {
			"V1gUNHs9Ae2t6hCA7MkVqghe2GWsJvB8exM",
			"V1oUJSYnwUoeowB2CykMBMg9RKRt6uUPoj2",
			"V1brKJBb8Q3HxPk3by7o6aNFjTtDhMWtEmX",
			"V1iQ8LDi8QS7PJjpDWnyPyaCA93sC1NZL5f",
			"V1TgZrkism6Zj7RPh4NoZbbXEdEWLpa6SMp",
			"V1fSF2pbMdDCE6tgG1waCG7e71NWSpKcYiX",
			"V1e7vjV8esvkdvokVqdwR7Bui3bD9YLZFWT",
			"V1RVmTC7mCHFM9iGQ5yZKYEDMynhw3bDFJ2",
			"V1gsR99Sd2azGwM5wNarCEFF4SvESL7yxjf",
			"V1SXJJn6mhD6CE7W8jXdS4rxc3B9dMkYghK",
			"V1R7AFHw6U5btTrxkVkeq2ZMS5KN7HrNj5A",
			"V1j957XzHXDRdz9ainnnjrnRqfSf3XBGvL4",
			"V1b7wFTbsuQBhGgAF5f5Dt8paPazSsrFTg4",
			"V1fgNT2qyA9G8VnEHCtMV1SFsJhWVgAEgoc",
			"V1YxmmRWxTKBNSB6Pv235o16ffgWRk9ZqrY",
			"V1YuCVe2ufQHUu3NS67bcbixUY9TbfmpNaW",
			"V1Y5g3Pm2Wn7QTzytrzFxiydKwdLVV9GXip",
			"V1hNZdZVjyUcfXYfu71R2RBjmyN8kZoRE7r",
			"V1at7HqpMbffzkxCeGRZk4nrK9rsWtGdo4M",
			"V1h7Fc3XxgCYS8CW26oDf5idQiNgCQMcMqm",
			"V1bwTWLiwe23K1avvYQzGt7Ffxr1p4KeAih",
			"V1Zrkrrh3BDWqnuBfS8Y5tBAse7xM3F1wAS"
		};
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        strCurrencyUnits = "TVEC";
        bip44CoinType = 1;
        consensus.fCoinbaseMustBeProtected = false;
        consensus.nSubsidySlowStartInterval = 0;
        consensus.nSubsidyHalvingInterval = 0;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 400;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 2;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 30; // 30% adjustment down
        consensus.nPowMaxAdjustUp = 10; // 10% adjustment up
        consensus.nPowTargetSpacing = 30;
        //consensus.nPowAllowMinDifficultyBlocksAfterHeight = 3000;
        consensus.nPowAllowMinDifficultyBlocksAfterHeight = boost::none;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nActivationHeight =
            Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nProtocolVersion = 170003;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nActivationHeight = 1000;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nProtocolVersion = 170007;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nActivationHeight = 1000;

        // The best chain should have at least this much work.
		consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
		
        pchMessageStart[0] = 0x57; // V+1
        pchMessageStart[1] = 0x46; // E+1
        pchMessageStart[2] = 0x44; // C+1
        pchMessageStart[3] = 0x55; // T+1
        vAlertPubKey = ParseHex("73B0");
        nDefaultPort = 25802;
        nPruneAfterHeight = 1000;
        const size_t N = 192, K = 7;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;

        genesis = CreateGenesisBlock(
            1548115200,
            uint256S("0000000000000000000000000000000000000000000000000000000000000003"),
            ParseHex("0286c4d1a9a58f1432d8075820764042e6a1b565765595405c0ecc850c696a0e31768eba37c2dea747f48fb107706fb3ac3904777e2f6250580cc1d3713762faf191ba0be54ec067bad2c82b868b232e097742787e9832f7ed919dbbd9fab3171f77ca800402462357e65b74a1b7d5747091fe2b9193b1326165ddece311d182fd72c38fde613f3763e39597c7e4e17286ac49e3ece906a66f099c706c984efdd30cb552b1536e850f55331bf5767d1eb8edea2e7c779067bbd57879c3891d20a4ead8b06189d0fa038165c5793ecd72e527aa8b457ed17c6057b6fbcb0dc3bd062b5b1aa50ebee9e0ddf5537572bd3e11e20c6ccd7b4ff1c0f011e7f0ef7cf28c34ccd0c1f9f5e534e5799d2ec53e736e62ba15b6a93e281be52d6c3c6523d4e3e10e0a314d77545751abae0a31e38558ce032212d02fd27207243af160933ed9ddbf52bf0f8ee93d573a8dbb0ccd1eca12c12bd3da0f7668fc69857dd76a061dba9e9a9dabdffee0e5a7c7f89649d08f939bafced26782735453963b67ac4035af04485c7fd7bf3a7ac97657fe2997"),
            0x200A1FA<<4, 4, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("06bb79575e1c22e2b4b1f13c2e6ed4f00c8cc84216375cfe7d01bc9f63dc9ea8"));

        vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("vectorium.org", "testnetseed.vectorium.org")); // no testnet seeds

        // guarantees the first 2 characters, when base58 encoded, are "tm"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x1D,0x25};
        // guarantees the first 2 characters, when base58 encoded, are "t2"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x1C,0xBA};
        // the first character, when base58 encoded, is "9" or "c" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0xEF};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x35,0x87,0xCF};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x35,0x83,0x94};
        // guarantees the first 2 characters, when base58 encoded, are "zt"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0xB6};
        // guarantees the first 4 characters, when base58 encoded, are "ZiVt"
        base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAC,0x0C};
        // guarantees the first 2 characters, when base58 encoded, are "ST"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAC,0x08};

        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "ztestsapling";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "zviewtestsapling";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "zivktestsapling";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "secret-extended-key-test";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;


        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (0, consensus.hashGenesisBlock)
        };

        // Founders reward script expects a vector of 2-of-3 multisig addresses
        vFoundersRewardAddress = {};
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());
        
        // Licensed miners
        consensus.nMinerLicenseActivationHeight = 4200;
		vLicensedMiners = {
			"tmGYsG7Xux1VzjWMWcR4r6AfPXRdxbnDixP",
			"tmRoDACm8Du6AaVtiL8dExQPXYkYR1TyWDE",
			"tmD4hDRe6n8j96PgbshRLSfw2eQEwYhyRk9",
			"tmTD7ARusdZoe5fub9kLwbqoPhthYxF3S3C",
			"tmQNp3gsyKFEQxrYzDJ9ju1jBW4S1gFg6QP",
			"tmKMfe7GLXKfhZ1GuZmpcdMU6bHyeHiLA6q",
			"tmD5ngr5YBhWMbiGqcSWGsowxuZxHmWSK6H",
			"tmNcNsDNMiZGBTQ3FLp1voJdfYCZXGfXSxr",
			"tmHABLhavt1Z5124UTWXGkkCG7kt8T72CnR",
			"tmFSoh6hehcWPPNPw6r1Ha4Gx9D3fMKrmW2",
			"tmBJU9MRbCTuRcUE3TgzAStQkmuyhei3ekK",
			"tmLR2TaN2JwgreWPtkoG3agCa3xBaY4FFxZ"
		};
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        strCurrencyUnits = "REG";
        bip44CoinType = 1;
        consensus.fCoinbaseMustBeProtected = false;
        consensus.nSubsidySlowStartInterval = 0;
        consensus.nSubsidyHalvingInterval = 0;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f");
        consensus.nPowAveragingWindow = 17;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 0; // Turn off adjustment down
        consensus.nPowMaxAdjustUp = 0; // Turn off adjustment up
        consensus.nPowTargetSpacing = 30;
        consensus.nPowAllowMinDifficultyBlocksAfterHeight = 0;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nActivationHeight =
            Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nProtocolVersion = 170003;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nProtocolVersion = 170006;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        pchMessageStart[0] = 0xaa;
        pchMessageStart[1] = 0xbb;
        pchMessageStart[2] = 0xcc;
        pchMessageStart[3] = 0xdd;
        vAlertPubKey = ParseHex("73B0");
        nDefaultPort = 25803;
        nPruneAfterHeight = 1000;
        const size_t N = 48, K = 5;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;

       genesis = CreateGenesisBlock(
            1548115201,
            uint256S("000000000000000000000000000000000000000000000000000000000000000e"),
            ParseHex("0cfa835f22fd9cf29f27dbaa18d58411d9a919c8da4f23c632b1e620f26c3ec45bede9e2"),
            0x200f0f0f, 4, 0);
		
        consensus.hashGenesisBlock = genesis.GetHash();
		assert(consensus.hashGenesisBlock == uint256S("0cabe5d2cbd65cfba422d043cabd98032633bedbb5ed8b674a0c6bad409fca56"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (0, consensus.hashGenesisBlock)
        };
        
        // These prefixes are the same as the testnet prefixes
        base58Prefixes[PUBKEY_ADDRESS]     = {0x1D,0x25};
        base58Prefixes[SCRIPT_ADDRESS]     = {0x1C,0xBA};
        base58Prefixes[SECRET_KEY]         = {0xEF};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x35,0x87,0xCF};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x35,0x83,0x94};
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0xB6};
        base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAC,0x0C};
        base58Prefixes[ZCSPENDING_KEY]     = {0xAC,0x08};

        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "zregtestsapling";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "zviewregtestsapling";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "zivkregtestsapling";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "secret-extended-key-regtest";

        // Founders reward script expects a vector of 2-of-3 multisig addresses
        vFoundersRewardAddress = {};
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());
    }

    void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
    {
        assert(idx > Consensus::BASE_SPROUT && idx < Consensus::MAX_NETWORK_UPGRADES);
        consensus.vUpgrades[idx].nActivationHeight = nActivationHeight;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);

    // Some python qa rpc tests need to enforce the coinbase consensus rule
    if (network == CBaseChainParams::REGTEST && mapArgs.count("-regtestprotectcoinbase")) {
        regTestParams.SetRegTestCoinbaseMustBeProtected();
    }
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}


// Block height must be >0 and <=last founders reward block height
// Index variable i ranges from 0 - (vFoundersRewardAddress.size()-1)
std::string CChainParams::GetFoundersRewardAddressAtHeight(int nHeight) const {
    int maxHeight = consensus.GetLastFoundersRewardBlockHeight();
    assert(nHeight > 0 && nHeight <= maxHeight);

    size_t addressChangeInterval = (maxHeight + vFoundersRewardAddress.size()) / vFoundersRewardAddress.size();
    size_t i = nHeight / addressChangeInterval;
    return vFoundersRewardAddress[i];
}

// Block height must be >0 and <=last founders reward block height
// The founders reward address is expected to be a multisig (P2SH) address
CScript CChainParams::GetFoundersRewardScriptAtHeight(int nHeight) const {
    assert(nHeight > 0 && nHeight <= consensus.GetLastFoundersRewardBlockHeight());

    CTxDestination address = DecodeDestination(GetFoundersRewardAddressAtHeight(nHeight).c_str());
    assert(IsValidDestination(address));
    assert(boost::get<CScriptID>(&address) != nullptr);
    CScriptID scriptID = boost::get<CScriptID>(address); // address is a boost variant
    CScript script = CScript() << OP_HASH160 << ToByteVector(scriptID) << OP_EQUAL;
    return script;
}

std::string CChainParams::GetFoundersRewardAddressAtIndex(int i) const {
    assert(i >= 0 && i < vFoundersRewardAddress.size());
    return vFoundersRewardAddress[i];
}

bool CChainParams::IsLicensedMiner(std::string address) const {
	auto result = std::find(std::begin(vLicensedMiners), std::end(vLicensedMiners), address);
	return (result != std::end(vLicensedMiners));
}

void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    regTestParams.UpdateNetworkUpgradeParameters(idx, nActivationHeight);
}
