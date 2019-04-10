#pragma once

#include <string>
#include <vector>
#include <stdexcept>

// VeriBlock NodeCore
// Copyright 2017-2019 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace org {
    namespace veriblock {
        namespace core {
            namespace utilities {


                class BlockUtility final {
                private:
                    BlockUtility();

                    static std::vector<int> const MASKS;

                    static constexpr int HEADER_SIZE = 64;

                    static constexpr int BLOCK_HEIGHT_START_POSITION = 0;
                    static constexpr int BLOCK_HEIGHT_END_POSITION = 3;

                    static constexpr int VERSION_START_POSITION = 4;
                    static constexpr int VERSION_END_POSITION = 5;

                    static constexpr int PREVIOUS_BLOCK_HASH_START_POSITION = 6;
                    static constexpr int PREVIOUS_BLOCK_HASH_END_POSITION = 17;

                    static constexpr int SECOND_PREVIOUS_BLOCK_HASH_START_POSITION = 18;
                    static constexpr int SECOND_PREVIOUS_BLOCK_HASH_END_POSITION = 26;

                    static constexpr int THIRD_PREVIOUS_BLOCK_HASH_START_POSITION = 27;
                    static constexpr int THIRD_PREVIOUS_BLOCK_HASH_END_POSITION = 35;

                    static constexpr int MERKLE_ROOT_HASH_START_POSITION = 36;
                    static constexpr int MERKLE_ROOT_HASH_END_POSITION = 51;

                    static constexpr int TIMESTAMP_START_POSITION = 52;
                    static constexpr int TIMESTAMP_END_POSITION = 55;

                    static constexpr int DIFFICULTY_START_POSITION = 56;
                    static constexpr int DIFFICULTY_END_POSITION = 59;

                    static constexpr int NONCE_START_POSITION = 60;
                    static constexpr int NONCE_END_POSITION = 63;

                    /**
                     * Assembles the compact-format block header, which is used for PoW mining and is published during the PoP process
                     * to Bitcoin.
                     * <p>
                     * The block header is 64 bytes which to fit comfortably into an 80-byte OP_RETURN on Bitcoin; remaining 16 bytes
                     * will be PoP miner identification data
                     *
                     * @param blockHeight              4-byte block height
                     * @param version                  2-byte block version
                     * @param previousBlockHash        12-byte previous block hash encoded as hexadecimal, hash of the previous block which this block builds upon
                     * @param secondPreviousBlockHash  9-byte second previous block hash encoded as hexadecimal
                     * @param thirdPreviousBlockHash   9-byte third previous block hash encoded as hexadecimal
                     * @param merkleRootHash           16-byte merkle root hash encoded as hexadecimal
                     * @param timestamp                4-byte timestamp in Unix epoch format (with second granularity)
                     * @param difficulty               4-byte difficulty
                     * @param nonce                    4-byte nonce used to achieve a block hash under target
                     * @return The block header created from all provided parameters
                     */
                public:
                    static std::vector<char>
                    assembleBlockHeader(int blockHeight, short version, const std::wstring &previousBlockHash,
                                        const std::wstring &secondPreviousBlockHash,
                                        const std::wstring &thirdPreviousBlockHash, const std::wstring &merkleRootHash,
                                        int timestamp, int difficulty, int nonce);

                    /**
                     * Determine whether the provided byte[] is a plausible block header, meaning it:
                     * - Is the correct length
                     * - vBlake hashes to fit under it's embedded difficulty
                     * @param blockHeader
                     * @return
                     */
                    static bool isPlausibleBlockHeader(std::vector<char> &blockHeader);

                    /**
                     * Determine whether a block header's PoW hash is below it's embedded difficulty.
                     * NOTE: This method only ensures that the block is self-consistent, but not that it is at the appropriate difficulty
                     * given the context of the blocks before it.
                     * @param blockHeader Block header to calculate the PoW hash of and compare to it's embedded difficulty
                     * @return Whether or not this block header's hash meets it's embedded difficulty
                     */
                    static bool isMinerHashBelowTarget(std::vector<char> &blockHeader);

                    /**
                     * Encodes a BigInteger value in compact bits form
                     * @param value BigInteger
                     * @return Compacted value as a long
                     */
                    static long long encodeCompactBits(BigInteger *value);

                    /**
                     * Extracts the integer block height from a given block header
                     * @param blockHeader Block header to extract the block height from
                     * @return Extracted integer block height
                     */
                    static int extractBlockHeightFromBlockHeader(std::vector<char> &blockHeader);

                    /**
                     * Extracts the short version from a given block header
                     * @param blockHeader Block header to extract the version from
                     * @return Extracted integer version
                     */
                    static short extractVersionFromBlockHeader(std::vector<char> &blockHeader);

                    /**
                     * Extracts the bytes of the previous block hash from a given block header
                     * @param blockHeader Block header to extract the previous block hash from
                     * @return Extracted previous block hash
                     */
                    static std::vector<char> extractPreviousBlockHashFromBlockHeader(std::vector<char> &blockHeader);

                    /**
                     * Extracts the bytes of the second previous block hash from a given block header
                     * @param blockHeader Block header to extract the second previous block hash from
                     * @return Extracted second previous block hash
                     */
                    static std::vector<char>
                    extractSecondPreviousBlockHashFromBlockHeader(std::vector<char> &blockHeader);

                    /**
                     * Extracts the bytes of the third previous block hash from a given block header
                     * @param blockHeader Block header to extract the third previous block hash from
                     * @return Extracted third previous block hash
                     */
                    static std::vector<char>
                    extractThirdPreviousBlockHashFromBlockHeader(std::vector<char> &blockHeader);

                    /**
                     * Extracts the bytes of the merkle root from a given block header
                     * @param blockHeader Block header to extract the merkle root from
                     * @return Extracted merkle root
                     */
                    static std::vector<char> extractMerkleRootFromBlockHeader(std::vector<char> &blockHeader);

                    /**
                     * Extracts the integer timestamp from a given block header
                     * @param blockHeader Block header to extract the timestamp from
                     * @return Extracted integer timestamp
                     */
                    static int extractTimestampFromBlockHeader(std::vector<char> &blockHeader);

                    /**
                     * Extracts the integer difficulty from a given block header
                     * @param blockHeader Block header to extract the difficulty from
                     * @return Extracted integer difficulty
                     */
                    static int extractDifficultyFromBlockHeader(std::vector<char> &blockHeader);

                    /**
                     * Extracts the integer nonce from a given block header
                     * @param blockHeader Block header to extract the nonce from
                     * @return Extracted integer nonce
                     */
                    static int extractNonceFromBlockHeader(std::vector<char> &blockHeader);

                private:
                    static std::vector<char> extractFromBlockHeader(std::vector<char> &blockHeader, int offset,
                                                                    int length) throw(std::invalid_argument);

                public:
                    static std::wstring hashBlock(std::vector<char> &blockHeader);
                };

            }
        }
    }
}
