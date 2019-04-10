#pragma once

#include <vector>
#include <limits>

// VeriBlock NodeCore
// Copyright 2017-2018 VeriBlock, Inc.
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {
    /**
     * This code is entirely mine (Maxwell Sanchez), and was originally released in late 2015 under MIT licensing.
     * It can easily be replaced with something more powerful.
     * <p>
     * This is an extremely simple class to introduce additional entropy based on unpredictable
     * computational time of variable-length computational routines.
     * <p>
     * Computational 'events' (the wasteTime() method) act to do an unknown amount of work,
     * so that time gap can be measured and used as entropy for the system. Even computations
     * of a known length and difficulty take variable amounts of time given the nature of
     * modern _runtime environments. Timing the _runtime of these events allows the introduction
     * of entropy based on properties outside the program, such as processor load, computer
     * performance, and thread scheduling.
     * <p>
     * Furthermore, available environment states (such as memory availability) provide additional
     * entropy.
     * <p>
     * At its core, this class utilizes Java's Random object to further mix internal state
     * and easily provide non-biased numbers. All top-level state modifications are based
     * on XOR, as XOR does not favor either zeroes or ones, and so will not cause incremental
     * decay towards a lower-entropy state.
     * <p>
     * This class focuses far more on the unpredictability of output rather than the even
     * distribution of results, but experimentally this class has shown to produce well-distributed
     * results.
     * <p>
     * In 1,000 trials, each consisting of averaging the result from 100,000 calls to Entropy.nextDouble()
     * and Random.nextDouble() (both initialized with no constructor arguments), the averages of
     * Entropy tended to be, on average, moderately farther away from the expected value of 0.5.
     * However, All differences from both classes were less than 0.8% from the expected value, which
     * for most purposes is a negligible bias. Additionally, biases did not favor either above
     * or below the expected value.
     * <p>
     * System.nanoTime() used for additional granularity in timing unknown-length computational
     * events.
     * <p>
     * TODO: Improve entropy gathering, add things like network latency, CPU temp (passthrough to a C-family library?),
     * possibly use secure entropy that modern CPUs provide based on external conditions (RDRAND), maybe use /dev/random and/or
     * /dev/urandom on *nix, available system memory, etc. Improving this class will reduce the chance of side-channel attacks
     * against users who use the default implementation to produce public/private keypairs. Private keys are only as
     * secure as the way in which they are generated, and the way in which that can be reproduced/front-run.
     *
     */
    class Entropy {
        // Updated whenever wasteTime() is called, to introduce additional entropy from variables
        // used for consuming CPU cycles in calculations.
    private:
        long long _stateMixinFromWaste = 0x3955170223037924LL;

        // State variables for default initialization values of the Entropy object
        // All have an equal number of ones and zeroes when represented in binary
        long long _state1 = 0x967ca962dd134c55LL;
        long long _state2 = 0x8e678ec4fa4a3721LL;
        long long _state3 = 0x741677f32bf14850LL;
        long long _state4 = 0x82359b9bbd5e1708LL;

        // The initial instantiation time of an Entropy object
        long long _startTimeNS = 0;

        /**
         * Constructor sets initial states dependent on timing and avalanche timing several related
         * computational events.
         * <p>
         * Results of these timings are used to set a reasonably mixed initial state well-removed
         * from start-time seeding.
         */
    public:
        virtual ~Entropy() {
        }

        Entropy();

        /**
         * Adds additional event-time-based mixing to the state.
         * Uses same event (wasteTime()) as constructor.
         * Called after every method call which pushes entropy to the outside world
         * (nextLong(), and all dependent methods).
         */
        virtual void entropize();

        /**
         * Allows additional entropy to be introduced from an external source.
         *
         * @param entropy byte[] representation of external entropy.
         */
        virtual void addEntropy(std::vector<char> &entropy);

        /**
         * Returns a pseudorandom boolean value based on present entropy.
         *
         * @return boolean A pseudorandom boolean value based on present entropy.
         */
        virtual bool nextBoolean();

        /**
         * Fills the provided byte array with psuedorandomly generated bytes.
         */
        virtual void nextBytes(std::vector<char> &bytes);

        /**
         * Returns a pseudorandom double value based on present entropy.
         *
         * @return double A pseudorandom double value based on present entropy.
         */
    private:
        double nextDouble();

        /**
         * Returns a pseudorandom float value based on present entropy.
         *
         * @return float A pseudorandom float value based on present entropy.
         */
    public:
        virtual float nextFloat();

        /**
         * Returns a pseudorandom long value based on present entropy.
         *
         * @return long A pseudorandom long value based on present entropy.
         */
        virtual long long nextLong();

        /**
         * Returns a pseudorandom long value based on present entropy below the provided limit.
         *
         * @param limit The (non-inclusive) limit of the returned long.
         * @return long A pseudorandom long value based on present entropy below the provided limit.
         */
        virtual long long nextLong(long long limit);

        /**
         * Returns a pseudorandom int value based on present entropy.
         *
         * @return int A pseudorandom int value based on present entropy.
         */
    private:
        int nextInt();

        /**
         * Returns a pseudorandom int value based on present entropy below the provided limit.
         *
         * @param limit The (non-inclusive) int of the returned int.
         * @return int A pseudorandom int value based on present entropy below the provided limit.
         */
        int nextInt(int limit);

        /**
         * A method designed to take an unknown (but miniscule) amount of time to complete.
         * Also adds slightly to the entropy of the object.
         */
        void wasteTime();
    };
}
