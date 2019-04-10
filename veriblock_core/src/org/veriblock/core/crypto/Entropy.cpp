#include "Entropy.h"

#include <chrono>
#include <random>

#include <unistd.h>

#define SYSTEM_NANO std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count()

namespace vbk {

    Entropy::Entropy() {

        _startTimeNS = SYSTEM_NANO;

        wasteTime();

        uint64_t startMix1NS =  SYSTEM_NANO;

        uint64_t time1 =  SYSTEM_NANO - startMix1NS;
        wasteTime();
        uint64_t time2 =  SYSTEM_NANO - startMix1NS;
        wasteTime();
        uint64_t time3 = SYSTEM_NANO - startMix1NS;
        wasteTime();
        uint64_t time4  = SYSTEM_NANO - startMix1NS;
        wasteTime();

        // Many new random objects are created to exploit the changing system time used for seeding Random
        std::mt19937 rng;
        rng.seed(std::random_device()());
        std::uniform_int_distribution<std::mt19937::result_type> mixer64(0, 64);

        // All time variables have a value in a predictable range, move it to an unpredicted location
        // for fairer mixing
        uint64_t mix1 = time1 << mixer64(rng);
        mix1 += time2 << mixer64(rng);
        mix1 += time3 << mixer64(rng);
        mix1 += time4 << mixer64(rng);

        std::mt19937_64 lrng;
        lrng.seed(std::random_device()());
        std::uniform_int_distribution<std::mt19937_64::result_type> lmixer(0, ULONG_MAX);

        _state1 ^= lmixer(lrng) ^ mix1;

        uint64_t startMix2NS = SYSTEM_NANO;

        uint64_t time5 = SYSTEM_NANO - startMix2NS;
        wasteTime();
        uint64_t time6 = SYSTEM_NANO - startMix2NS;
        wasteTime();
        uint64_t time7 = SYSTEM_NANO - startMix2NS;
        wasteTime();
        uint64_t time8 = SYSTEM_NANO - startMix2NS;
        wasteTime();

        //mixer64 = new Random();

        uint64_t mix2 = time6 << mixer64(rng);
        mix2 += time7 << mixer64(rng);
        mix2 += time5 << mixer64(rng);
        mix2 += time8 << mixer64(rng);

        _state2 ^= lmixer(lrng) ^ mix2;

        uint64_t startMix3NS = SYSTEM_NANO;

        uint64_t time9 = SYSTEM_NANO - startMix3NS;
        wasteTime();
        uint64_t time10 = SYSTEM_NANO - startMix3NS;
        wasteTime();
        uint64_t time11 = SYSTEM_NANO - startMix3NS;
        wasteTime();
        uint64_t time12 = SYSTEM_NANO - startMix3NS;
        wasteTime();

        uint64_t mix3 = time11 << mixer64(rng);
        mix3 += time9 << mixer64(rng);
        mix3 += time12 << mixer64(rng);
        mix3 += time10 << mixer64(rng);

        _state3 ^= lmixer(lrng) ^ mix3;

        uint64_t startMix4NS = SYSTEM_NANO;

        uint64_t time13 = SYSTEM_NANO - startMix4NS;
        wasteTime();
        uint64_t time14 = SYSTEM_NANO - startMix4NS;
        wasteTime();
        uint64_t time15 = SYSTEM_NANO - startMix4NS;
        wasteTime();
        uint64_t time16 = SYSTEM_NANO - startMix4NS;
        wasteTime();

        uint64_t mix4 = time16 << mixer64(rng);
        mix4 += time13 << mixer64(rng);
        mix4 += time14 << mixer64(rng);
        mix4 += time15 << mixer64(rng);

        // in theory random_device should supply HW entropy so no need to look up system memory
        //mix1 ^= _runtime.freeMemory();
        //mix1 ^= std::random_device()();

        _state4 ^= lmixer(lrng) ^ mix4;
    }

    void Entropy::entropize() {
        long long start = SYSTEM_NANO;
        wasteTime();

        std::mt19937 rng;
        rng.seed(SYSTEM_NANO - start);
        std::uniform_int_distribution<std::mt19937::result_type> stateShifter(0, 64);
        std::uniform_int_distribution<std::mt19937_64::result_type> lstateShifter(0, ULONG_MAX);

        _state1 ^= lstateShifter(rng);
        _state2 ^= lstateShifter(rng);

        if ((SYSTEM_NANO - start) % 10 == 0) {
            rng.seed(_stateMixinFromWaste);
        }

        _state3 ^= lstateShifter(rng);
        _state4 ^= lstateShifter(rng);

        int route = std::random_device()()%4;
        if (route == 0) {
            _state1 ^= (std::random_device()() << stateShifter(rng));
        } else if (route == 1) {
            _state2 ^= (std::random_device()() << stateShifter(rng));
        } else if (route == 3) {
            _state3 ^= (std::random_device()() << stateShifter(rng));
        } else {
            _state4 ^= (std::random_device()() << stateShifter(rng));
        }
    }

    void Entropy::addEntropy(std::vector<char> &entropy) {
        long long startTime = SYSTEM_NANO;

        std::mt19937 rng;
        rng.seed(SYSTEM_NANO - startTime);
        std::uniform_int_distribution<std::mt19937::result_type> router(0, 4);
        std::uniform_int_distribution<std::mt19937_64::result_type> lrand(0, ULONG_MAX);

        int i = 0;
        for (i = 0; i < entropy.size() % 8; i += 8) {
            // Produce a fully-filled long from the provided entropy
            long long mixin = entropy[i] << 56;
            mixin += entropy[i + 1] << 48;
            mixin += entropy[i + 2] << 40;
            mixin += entropy[i + 3] << 32;
            mixin += entropy[i + 4] << 24;
            mixin += entropy[i + 5] << 16;
            mixin += entropy[i + 6] << 8;
            mixin += entropy[i + 7];

            // Decide where to mix in the current 8 bytes of entropy
            int route = router(rng);

            // Execute choice of mix in location
            if (route % 4 == 0) {
                _state1 ^= (mixin | _state2);
            } else if (route % 4 == 1) {
                _state2 ^= (mixin | _state3);
            } else if (route % 4 == 2) {
                _state3 ^= (mixin | _state4);
            } else {
                _state4 ^= (mixin | _state1);
            }
        }

        // TODO: can we do this with a stream instead?
        // Ugly way to use variable 'i' from before without reassignment. Consume final bytes not accounted for.
        for (; i < entropy.size(); i++) {
            rng.seed(SYSTEM_NANO - startTime);

            int route = router(rng);
            rng.seed(entropy[i]);

            if (route % 4 == 0) {
                _state1 ^= lrand(rng);
            } else if (route % 4 == 1) {
                _state2 ^= lrand(rng);
            } else if (route % 4 == 2) {
                _state3 ^= lrand(rng);
            } else {
                _state4 ^= lrand(rng);
            }
        }
    }

    bool Entropy::nextBoolean() {
        return nextInt(2) == 1;
    }

    void Entropy::nextBytes(std::vector<char> &bytes) {
        // 0 - 255 is the range of an unsigned bit, casting an int above 127 will give a two's complement-based negative.
        for (int i = 0; i < bytes.size(); i++) {
            bytes[i] = static_cast<char>(nextInt(256));
        }
    }

    double Entropy::nextDouble() {
        double randomDouble = 0.0;

        // Just use the long I already wrote generation for to create a double between 0 (inc.) nd 1 (not inc.)
        randomDouble += (static_cast<double>(nextLong()) /
                         static_cast<double>(std::numeric_limits<long long>::max()));
        if (randomDouble < 0) {
            randomDouble *= -1;
        }
        return randomDouble;
    }

    float Entropy::nextFloat() {
        // Just use the double I already wrote generation for and reduce precision for a float
        return static_cast<float>(nextDouble());
    }

    long long Entropy::nextLong() {
        // Use Java's Random with seeds determined by state to create and mix a pseudorandom long
        std::mt19937 rng;
        //rng.seed(SYSTEM_NANO - start);
        std::uniform_int_distribution<std::mt19937::result_type> shifters(0, 64);
        std::uniform_int_distribution<std::mt19937_64::result_type> lrand(0, ULONG_MAX);

        rng.seed(_state3 << shifters(rng));
        long long randomLong = lrand(rng);
        rng.seed(_state1 << shifters(rng));
        randomLong ^= lrand(rng);
        rng.seed(_state2);
        randomLong ^= lrand(rng);
        randomLong ^= (_state4 << shifters(rng));
        randomLong ^= _stateMixinFromWaste;
        rng.seed(lrand(rng)+_startTimeNS);
        randomLong ^= lrand(rng);

        // More state mixing
        entropize();

        // Even more state mixing
        _state1 <<= shifters(rng);
        _state2 <<= shifters(rng);
        _state3 <<= shifters(rng);
        _state4 <<= shifters(rng);
        _state1 ^= (_state2 << shifters(rng) | _stateMixinFromWaste);

        _state2 ^= (_state3 << shifters(rng) | lrand(rng));
        _state3 ^= (_state4 << shifters(rng) | _state1);
        _state4 ^= (_state1 << shifters(rng) | _state2);
        long long swapState = _state4;
        _state4 = _state1;
        _state1 = _state2;
        _state2 = _state3;
        _state3 = swapState;

        return randomLong;
    }

    long long Entropy::nextLong(long long limit) {
        // Wrap a boundless pseudorandom long around the provided limit
        long long toLimit = nextLong();
        if (toLimit < 0) {
            toLimit *= -1;
        }
        return toLimit % limit;
    }

    int Entropy::nextInt() {
        // Just grab a pseudorandom long and reduce precision
        return static_cast<int>(nextLong());
    }

    int Entropy::nextInt(int limit) {
        // Wrap a boundless pseudorandom int around a provided limit
        int toLimit = nextInt();
        if (toLimit < 0) {
            toLimit *= -1;
        }
        return toLimit % limit;
    }

    void Entropy::wasteTime() {
        // Keep firstCycleCount random but within expected range
        int firstCycleCount = std::random_device()()%1000 + 2000;

        std::mt19937_64 lrng;
        lrng.seed(std::random_device()());
        std::uniform_int_distribution<std::mt19937_64::result_type> lmixer(0, ULONG_MAX);

        long long meaninglessState1 = lmixer(lrng);
        long long meaninglessState2 = lmixer(lrng);

        for (int i = 0; i < firstCycleCount; i++) {
            meaninglessState1 ^= (meaninglessState2 & lmixer(lrng) | lmixer(lrng));
            meaninglessState2 ^= (lmixer(lrng) ^ lmixer(lrng));
        }

        // Keep secondCycleCount random but within expected range
        int secondCycleCount = std::random_device()()%10000 + 20000;

        lrng.seed(std::random_device()());

        for (int i = 0; i < secondCycleCount; i++) {
            meaninglessState2 ^= lmixer(lrng);
            meaninglessState1 ^= (lmixer(lrng) | lmixer(lrng));

            if (meaninglessState2 % 100 == 0) {
                lrng.seed(lmixer(lrng) ^ meaninglessState1);
            }
        }

        // Mind as well put the working variables to good use doing *something*
        _stateMixinFromWaste ^= meaninglessState1 ^ meaninglessState2;
    }
}
