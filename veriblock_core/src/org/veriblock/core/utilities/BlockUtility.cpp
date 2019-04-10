#include "BlockUtility.h"
#include "../crypto/Crypto.h"
#include "../bitcoinj/BitcoinUtilities.h"
#include "../SharedConstants.h"

namespace org {
    namespace veriblock {
        namespace core {
            namespace utilities {
                using SharedConstants = org::veriblock::core::SharedConstants;
                using BitcoinUtilities = org::veriblock::core::bitcoinj::BitcoinUtilities;
                using Crypto = org::veriblock::core::crypto::Crypto;

                BlockUtility::BlockUtility() {
                }

                std::vector<int> const BlockUtility::MASKS = std::vector<int>{0xFF000000, 0x00FF0000, 0x0000FF00,
                                                                              0x000000FF};

                std::vector<char>
                BlockUtility::assembleBlockHeader(int blockHeight, short version, const std::wstring &previousBlockHash,
                                                  const std::wstring &secondPreviousBlockHash,
                                                  const std::wstring &thirdPreviousBlockHash,
                                                  const std::wstring &merkleRootHash, int timestamp, int difficulty,
                                                  int nonce) {
                    std::vector<char> header(NONCE_END_POSITION + 1);

                    int shift = 24;
                    for (int i = BLOCK_HEIGHT_START_POSITION; i <= BLOCK_HEIGHT_END_POSITION; i++) {
                        header[i] = static_cast<char>((blockHeight & MASKS[i - BLOCK_HEIGHT_START_POSITION]) >> shift);
                        shift -= 8;
                    }

                    shift = 8;
                    for (int i = VERSION_START_POSITION; i <= VERSION_END_POSITION; i++) {
                        header[i] = static_cast<char>((version & MASKS[i - VERSION_START_POSITION + 2]) >> shift);
                        shift -= 8;
                    }


                    previousBlockHash = previousBlockHash.substr(previousBlockHash.length() - (2 * (1 +
                                                                                                    PREVIOUS_BLOCK_HASH_END_POSITION -
                                                                                                    PREVIOUS_BLOCK_HASH_START_POSITION)));

                    secondPreviousBlockHash = secondPreviousBlockHash.substr(secondPreviousBlockHash.length() - (2 *
                                                                                                                 (1 +
                                                                                                                  SECOND_PREVIOUS_BLOCK_HASH_END_POSITION -
                                                                                                                  SECOND_PREVIOUS_BLOCK_HASH_START_POSITION)));

                    thirdPreviousBlockHash = thirdPreviousBlockHash.substr(thirdPreviousBlockHash.length() - (2 * (1 +
                                                                                                                   THIRD_PREVIOUS_BLOCK_HASH_END_POSITION -
                                                                                                                   THIRD_PREVIOUS_BLOCK_HASH_START_POSITION)));


                    for (int i = 0; i <= (PREVIOUS_BLOCK_HASH_END_POSITION - PREVIOUS_BLOCK_HASH_START_POSITION); i++) {
//JAVA TO C++ CONVERTER TODO TASK: Only single-argument parse and valueOf methods are converted:
//ORIGINAL LINE: header[i + PREVIOUS_BLOCK_HASH_START_POSITION] = (byte)(Integer.parseInt(previousBlockHash.substring(i * 2, ((i * 2) + 2) - (i * 2)), 16));
                        header[i + PREVIOUS_BLOCK_HASH_START_POSITION] = static_cast<char>(Integer::valueOf(
                                previousBlockHash.substr(i * 2, ((i * 2) + 2) - (i * 2)), 16));
                    }

                    for (int i = 0; i <= (SECOND_PREVIOUS_BLOCK_HASH_END_POSITION -
                                          SECOND_PREVIOUS_BLOCK_HASH_START_POSITION); i++) {
//JAVA TO C++ CONVERTER TODO TASK: Only single-argument parse and valueOf methods are converted:
//ORIGINAL LINE: header[i + SECOND_PREVIOUS_BLOCK_HASH_START_POSITION] = (byte)(Integer.parseInt(secondPreviousBlockHash.substring(i * 2, ((i * 2) + 2) - (i * 2)), 16));
                        header[i + SECOND_PREVIOUS_BLOCK_HASH_START_POSITION] = static_cast<char>(Integer::valueOf(
                                secondPreviousBlockHash.substr(i * 2, ((i * 2) + 2) - (i * 2)), 16));
                    }

                    for (int i = 0; i <= (THIRD_PREVIOUS_BLOCK_HASH_END_POSITION -
                                          THIRD_PREVIOUS_BLOCK_HASH_START_POSITION); i++) {
//JAVA TO C++ CONVERTER TODO TASK: Only single-argument parse and valueOf methods are converted:
//ORIGINAL LINE: header[i + THIRD_PREVIOUS_BLOCK_HASH_START_POSITION] = (byte)(Integer.parseInt(thirdPreviousBlockHash.substring(i * 2, ((i * 2) + 2) - (i * 2)), 16));
                        header[i + THIRD_PREVIOUS_BLOCK_HASH_START_POSITION] = static_cast<char>(Integer::valueOf(
                                thirdPreviousBlockHash.substr(i * 2, ((i * 2) + 2) - (i * 2)), 16));
                    }


                    for (int i = 0; i <= (MERKLE_ROOT_HASH_END_POSITION - MERKLE_ROOT_HASH_START_POSITION); i++) {
//JAVA TO C++ CONVERTER TODO TASK: Only single-argument parse and valueOf methods are converted:
//ORIGINAL LINE: header[i + MERKLE_ROOT_HASH_START_POSITION] = (byte)(Integer.parseInt(merkleRootHash.substring(i * 2, ((i * 2) + 2) - (i * 2)), 16));
                        header[i + MERKLE_ROOT_HASH_START_POSITION] = static_cast<char>(Integer::valueOf(
                                merkleRootHash.substr(i * 2, ((i * 2) + 2) - (i * 2)), 16));
                    }

                    shift = 24;
                    for (int i = TIMESTAMP_START_POSITION; i <= TIMESTAMP_END_POSITION; i++) {
                        header[i] = static_cast<char>((timestamp & MASKS[i - TIMESTAMP_START_POSITION]) >> shift);
                        shift -= 8;
                    }

                    shift = 24;
                    for (int i = DIFFICULTY_START_POSITION; i <= DIFFICULTY_END_POSITION; i++) {
                        header[i] = static_cast<char>((difficulty & MASKS[i - DIFFICULTY_START_POSITION]) >> shift);
                        shift -= 8;
                    }

                    shift = 24;
                    for (int i = NONCE_START_POSITION; i <= NONCE_END_POSITION; i++) {
                        header[i] = static_cast<char>((nonce & MASKS[i - NONCE_START_POSITION]) >> shift);
                        shift -= 8;
                    }

                    return header;
                }

                bool BlockUtility::isPlausibleBlockHeader(std::vector<char> &blockHeader) {
                    if (blockHeader.size() != NONCE_END_POSITION + 1) {
                        return false;
                    }

                    return isMinerHashBelowTarget(blockHeader);
                }

                bool BlockUtility::isMinerHashBelowTarget(std::vector<char> &blockHeader) {
                    Crypto *crypto = new Crypto();
                    std::wstring blockHash = crypto->vBlakeReturnHex(blockHeader);
                    BigInteger *difficulty = BitcoinUtilities::decodeCompactBits(
                            extractDifficultyFromBlockHeader(blockHeader));
                    if (difficulty->compareTo(BigInteger::ZERO) < 0) {
                        delete crypto;
                        throw std::invalid_argument("Difficulty extracted from block header is not valid");
                    }

                    BigInteger *target = SharedConstants::DIFFICULTY_CALCULATOR_MAXIMUM_TARGET->divide(
                            difficulty->multiply(static_cast<BigInteger>(1)));

                    delete crypto;
                    BigInteger tempVar(blockHash, 16);
                    return (&tempVar)->compareTo(target) < 0;
                }

                long long BlockUtility::encodeCompactBits(BigInteger *value) {
                    long long result;
                    int size = value->toByteArray()->length;
                    if (size <= 3) {
                        result = value->longValue() << 8 * (3 - size);
                    } else {
                        result = value->shiftRight(8 * (size - 3)).longValue();
                    }
                    // The 0x00800000 bit denotes the sign.
                    // Thus, if it is already set, divide the mantissa by 256 and increase the exponent.
                    if ((result & 0x00800000LL) != 0) {
                        result >>= 8;
                        size++;
                    }
                    result |= size << 24;
                    result |= value->signum() == -1 ? 0x00800000 : 0;
                    return result;
                }

                int BlockUtility::extractBlockHeightFromBlockHeader(std::vector<char> &blockHeader) {
                    std::vector<char> blockHeightBytes = extractFromBlockHeader(blockHeader,
                                                                                BLOCK_HEIGHT_START_POSITION,
                                                                                BLOCK_HEIGHT_END_POSITION -
                                                                                BLOCK_HEIGHT_START_POSITION + 1);

                    int blockHeight = 0;
                    int bytePointer = 0;
                    for (int shift = 24; shift >= 0; shift -= 8) {
                        blockHeight += (0xFF & (blockHeightBytes[bytePointer])) << shift;
                        bytePointer++;
                    }

                    return blockHeight;
                }

                short BlockUtility::extractVersionFromBlockHeader(std::vector<char> &blockHeader) {
                    std::vector<char> versionBytes = extractFromBlockHeader(blockHeader, VERSION_START_POSITION,
                                                                            VERSION_END_POSITION -
                                                                            VERSION_START_POSITION + 1);

                    short version = 0;
                    int bytePointer = 0;
                    for (int shift = 8; shift >= 0; shift -= 8) {
                        version += (0xFF & (versionBytes[bytePointer])) << shift;
                        bytePointer++;
                    }

                    return version;
                }

                std::vector<char>
                BlockUtility::extractPreviousBlockHashFromBlockHeader(std::vector<char> &blockHeader) {
                    return extractFromBlockHeader(blockHeader, PREVIOUS_BLOCK_HASH_START_POSITION,
                                                  PREVIOUS_BLOCK_HASH_END_POSITION -
                                                  PREVIOUS_BLOCK_HASH_START_POSITION + 1);
                }

                std::vector<char>
                BlockUtility::extractSecondPreviousBlockHashFromBlockHeader(std::vector<char> &blockHeader) {
                    return extractFromBlockHeader(blockHeader, SECOND_PREVIOUS_BLOCK_HASH_START_POSITION,
                                                  SECOND_PREVIOUS_BLOCK_HASH_END_POSITION -
                                                  SECOND_PREVIOUS_BLOCK_HASH_START_POSITION + 1);
                }

                std::vector<char>
                BlockUtility::extractThirdPreviousBlockHashFromBlockHeader(std::vector<char> &blockHeader) {
                    return extractFromBlockHeader(blockHeader, THIRD_PREVIOUS_BLOCK_HASH_START_POSITION,
                                                  THIRD_PREVIOUS_BLOCK_HASH_END_POSITION -
                                                  THIRD_PREVIOUS_BLOCK_HASH_START_POSITION + 1);
                }

                std::vector<char> BlockUtility::extractMerkleRootFromBlockHeader(std::vector<char> &blockHeader) {
                    return extractFromBlockHeader(blockHeader, MERKLE_ROOT_HASH_START_POSITION,
                                                  MERKLE_ROOT_HASH_END_POSITION - MERKLE_ROOT_HASH_START_POSITION + 1);
                }

                int BlockUtility::extractTimestampFromBlockHeader(std::vector<char> &blockHeader) {
                    std::vector<char> timestampBytes = extractFromBlockHeader(blockHeader, TIMESTAMP_START_POSITION,
                                                                              TIMESTAMP_END_POSITION -
                                                                              TIMESTAMP_START_POSITION + 1);

                    int timestamp = 0;
                    int bytePointer = 0;
                    for (int shift = 24; shift >= 0; shift -= 8) {
                        timestamp += (0xFF & (timestampBytes[bytePointer])) << shift;
                        bytePointer++;
                    }

                    return timestamp;
                }

                int BlockUtility::extractDifficultyFromBlockHeader(std::vector<char> &blockHeader) {
                    std::vector<char> difficultyBytes = extractFromBlockHeader(blockHeader, DIFFICULTY_START_POSITION,
                                                                               DIFFICULTY_END_POSITION -
                                                                               DIFFICULTY_START_POSITION + 1);

                    int difficulty = 0;
                    int bytePointer = 0;

                    for (int shift = 24; shift >= 0; shift -= 8) {
                        difficulty += (0xFF & (difficultyBytes[bytePointer])) << shift;
                        bytePointer++;
                    }

                    return difficulty;
                }

                int BlockUtility::extractNonceFromBlockHeader(std::vector<char> &blockHeader) {
                    std::vector<char> nonceBytes = extractFromBlockHeader(blockHeader, NONCE_START_POSITION,
                                                                          NONCE_END_POSITION - NONCE_START_POSITION +
                                                                          1);

                    int nonce = 0;
                    int bytePointer = 0;
                    for (int shift = 24; shift >= 0; shift -= 8) {
                        nonce += (0xFF & (nonceBytes[bytePointer])) << shift;
                        bytePointer++;
                    }

                    return nonce;
                }

                std::vector<char> BlockUtility::extractFromBlockHeader(std::vector<char> &blockHeader, int offset,
                                                                       int length) throw(std::invalid_argument) {
                    if (blockHeader.empty()) {
                        throw std::invalid_argument(
                                "extractFromBlockHeader cannot be called with a null block header!");
                    }

                    if (blockHeader.size() != HEADER_SIZE) {
                        throw std::invalid_argument(
                                "extractFromBlockHeader cannot be called with a block header that is " +
                                std::to_wstring(blockHeader.size()) + L" bytes, must be " +
                                std::to_wstring(HEADER_SIZE) + L"!");
                    }

                    std::vector<char> extracted(length);
                    System::arraycopy(blockHeader, offset, extracted, 0, length);
                    return extracted;
                }

                std::wstring BlockUtility::hashBlock(std::vector<char> &blockHeader) {
                    Crypto tempVar();
                    std::wstring blockHashUnchopped = (&tempVar)->vBlakeReturnHex(blockHeader);
                    return blockHashUnchopped.substr(0, SharedConstants::VBLAKE_HASH_OUTPUT_SIZE_BYTES *
                                                        2); // *2 to account for Hex
                }
            }
        }
    }
}
