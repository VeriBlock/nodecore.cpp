cmake_minimum_required(VERSION 3.12)

project(veriblockcore_lib)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

include_directories(src/org/veriblock/core)
include_directories(src/org/veriblock/core/bitcoin)
include_directories(src/org/veriblock/core/contracts)
include_directories(src/org/veriblock/core/crypto)
include_directories(src/org/veriblock/core/types)
include_directories(src/org/veriblock/core/utilities)
include_directories(src/org/veriblock/core/wallet)

    set(HEADER_FILES
            src/org/veriblock/core/bitcoin/Base58.h
            src/org/veriblock/core/bitcoin/Base59.h
            src/org/veriblock/core/bitcoin/BitcoinUtilities.h
        src/org/veriblock/core/contracts/Address.h
        src/org/veriblock/core/contracts/AddressManager.h
        src/org/veriblock/core/contracts/FileProvider.h
        src/org/veriblock/core/contracts/FilteredBlock.h
        src/org/veriblock/core/contracts/LedgerMerklePath.h
        src/org/veriblock/core/contracts/MerklePath.h
        src/org/veriblock/core/contracts/Output.h
        src/org/veriblock/core/contracts/PartialMerkleTree.h
        src/org/veriblock/core/contracts/TransactionAddress.h
        src/org/veriblock/core/contracts/TransactionAmount.h
        src/org/veriblock/core/contracts/WalletRepository.h
        src/org/veriblock/core/crypto/BloomFilter.h
        src/org/veriblock/core/bitcoin/sha256.h
        src/org/veriblock/core/crypto/Crypto.h
#        src/org/veriblock/core/crypto/DataMerkleTree.h
        src/org/veriblock/core/crypto/Entropy.h
        src/org/veriblock/core/crypto/MerkleLayer.h
#        src/org/veriblock/core/crypto/Murmur3.h
        src/org/veriblock/core/crypto/vBlake.h
        src/org/veriblock/core/types/Bitflag.h
        src/org/veriblock/core/types/Pair.h
        src/org/veriblock/core/types/SimpleResult.h
        src/org/veriblock/core/types/Triple.h
#        src/org/veriblock/core/utilities/AddressUtility.h
#        src/org/veriblock/core/utilities/BlockUtility.h
#        src/org/veriblock/core/utilities/SerializerUtility.h
#        src/org/veriblock/core/utilities/TransactionUtility.h
        src/org/veriblock/core/utilities/Utility.h
#        src/org/veriblock/core/wallet/DefaultAddress.h
#        src/org/veriblock/core/wallet/DefaultAddressManager.h
#        src/org/veriblock/core/wallet/FlatfileWalletRepository.h
        src/org/veriblock/core/AddressConstants.h
        src/org/veriblock/core/DefaultOutput.h
        src/org/veriblock/core/SharedConstants.h
        exceptionhelper.h
        stringbuilder.h
        stringhelper.h
        tangible_filesystem.h
        )

    set(SOURCE_FILES
            src/org/veriblock/core/bitcoin/Base58.cpp
            src/org/veriblock/core/bitcoin/Base59.cpp
#        src/org/veriblock/core/bitcoin/BitcoinUtilities.cpp
        src/org/veriblock/core/contracts/TransactionAddress.cpp
        src/org/veriblock/core/contracts/TransactionAmount.cpp
#        src/org/veriblock/core/crypto/BloomFilter.cpp
        src/org/veriblock/core/crypto/Crypto.cpp
        src/org/veriblock/core/bitcoin/sha256.cpp
            #        src/org/veriblock/core/crypto/DataMerkleTree.cpp
        src/org/veriblock/core/crypto/Entropy.cpp
        src/org/veriblock/core/crypto/MerkleLayer.cpp
#        src/org/veriblock/core/crypto/Murmur3.cpp
        src/org/veriblock/core/crypto/vBlake.cpp
        src/org/veriblock/core/types/Bitflag.cpp
        src/org/veriblock/core/types/SimpleResult.cpp
#        src/org/veriblock/core/utilities/AddressUtility.cpp
#        src/org/veriblock/core/utilities/BlockUtility.cpp
        src/org/veriblock/core/utilities/SerializerUtility.cpp
#        src/org/veriblock/core/utilities/TransactionUtility.cpp
        src/org/veriblock/core/utilities/Utility.cpp
#        src/org/veriblock/core/wallet/DefaultAddress.cpp
#        src/org/veriblock/core/wallet/DefaultAddressManager.cpp
#        src/org/veriblock/core/wallet/FlatfileWalletRepository.cpp
        src/org/veriblock/core/DefaultOutput.cpp
        src/org/veriblock/core/SharedConstants.cpp
        )

add_library(veriblockcore_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})