#include "MerkleLayer.h"
#include "Crypto.h"
#include "../utilities/Utility.h"

namespace vbk {
    using Utility = vbk::Utility;

    MerkleLayer::MerkleLayer(std::vector<std::vector<char>> &data) : _data(data) {
    }

    MerkleLayer *MerkleLayer::createNextLayer() {
        /* Create a 2D array for the new layer _data that is half the size (round up, if fractional) of this layer's _data */
        std::vector<std::vector<char>> newData(
                ((_data.size() % 2 == 0) ? _data.size() / 2 : _data.size() / 2 + 1));

        for (int i = 0; i < newData.size(); i++) {
            /* Element i of newData is SHA256D of the two corresponding elements beneath it. If only one element is left, use it as both leaves. */
            Crypto tempVar;
            std::vector<char> temp = Utility::concat(_data[i * 2],
                                                     ((_data.size() != i * 2 + 1) ? _data[i * 2 + 1]
                                                                                  : _data[i * 2]));

            newData[i] = tempVar.SHA256D(temp);
        }

        return new MerkleLayer(newData);
    }

    int MerkleLayer::numElementsInLayer() {
        return _data.size();
    }

    std::vector<char> MerkleLayer::getElement(int elementNum) {
        return _data[elementNum];
    }
}
