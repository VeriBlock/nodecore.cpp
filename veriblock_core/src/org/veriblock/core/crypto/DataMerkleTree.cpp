#include "DataMerkleTree.h"
#include "MerkleLayer.h"

namespace org {
    namespace veriblock {
        namespace core {
            namespace crypto {

                java::util::Base64::Encoder *const DataMerkleTree::_base64Encoder = java::util::Base64::getEncoder();
                java::util::Base64::Decoder *const DataMerkleTree::_base64Decoder = java::util::Base64::getDecoder();

                DataMerkleTree::DataMerkleTree(std::vector<std::wstring> &inputData) {
                    /* All of the data of the tree */
                    std::vector<std::vector<char>> floorData(inputData.size());

                    for (int i = 0; i < inputData.size(); i++) {
                        floorData[i] = _base64Decoder->decode(inputData[i]);
                    }

                    /* Create, at a minimum, the bottom floor */
                    MerkleLayer tempVar(floorData);
                    _layers.push_back(&tempVar);

                    buildTree();
                }

                void DataMerkleTree::buildTree() {
                    /* When the top layer has a single element, the tree is finished */
                    /* Create the next layer, save it above the current highest layer */
                    while (_layers[_layers.size() - 1]->numElementsInLayer() > 1) {
                        _layers.push_back(_layers[_layers.size() - 1]->createNextLayer());
                    }
                }

                std::wstring DataMerkleTree::getMerkleRoot() {
                    return _base64Encoder->encodeToString(_layers[_layers.size() - 1]->getElement(0));
                }
            }
        }
    }
}
