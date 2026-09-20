#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;

// Check whether a number is prime.
bool isPrime(long long x) {
    if (x < 2)
        return false;

    for (long long i = 2; i * i <= x; ++i) {
        if (x % i == 0)
            return false;
    }

    return true;
}

// Find the smallest prime greater than x.
long long findPrime(long long x) {
    x++;

    while (!isPrime(x))
        x++;

    return x;
}

// Universal hash function.
int hashFunction(int x, int a, int b, long long p, int m) {
    long long value = ((1LL * a * x + b) % p + p) % p;
    return value % m;
}

int main() {

    int N;
    cin>>N;

    vector<int> keys(N);

    int maximum;

    // Read the dataset and find its maximum key.
    for (int i = 0; i < N; ++i) {
        cin>>keys[i];
        if (i == 0 || keys[i] > maximum)
            maximum = keys[i];
    }

    int q = N / 20;

    vector<int> randomSet(q);

    for (int i = 0; i < q; ++i)
        cin >> randomSet[i];

    /*
       Remove duplicate elements from the dataset.
    */

    sort(keys.begin(), keys.end());

    keys.erase(
        unique(keys.begin(), keys.end()),
        keys.end()
    );

    int n = keys.size();

    /*
       Choose p greater than every dataset key.
    */

    long long p = findPrime(maximum);

    /*
       First-level hashing.

       Find a hash function such that:

       sum |bucket[i]|^2 <= 4n
    */

    srand(1);

    int a1;
    int b1;

    vector<vector<int> > bucket;

    while (true) {
        a1 = rand() % (p - 1) + 1;
        b1 = rand() % p;

        bucket.clear();
        bucket.resize(n);

        for (int i = 0; i < n; ++i) {
            int index =
                hashFunction(keys[i], a1, b1, p, n);

            bucket[index].push_back(keys[i]);
        }

        long long sum = 0;

        for (int i = 0; i < n; ++i) {
            sum +=
                1LL * bucket[i].size() * bucket[i].size();
        }

        if (sum <= 4LL * n)
            break;
    }

    /*
       Second-level hash tables.
    */

    vector<int> a2(n, 0);
    vector<int> b2(n, 0);

    vector<vector<int> > T(n);
    vector<vector<bool> > occupied(n);

    for (int i = 0; i < n; ++i) {

        int size =
            bucket[i].size() * bucket[i].size();

        if (size == 0)
            continue;

        /*
           If there is only one key,
           a table of size 1 is enough.
        */

        if (size == 1) {
            T[i].resize(1);
            occupied[i].resize(1, false);

            a2[i] = 1;
            b2[i] = 0;

            T[i][0] = bucket[i][0];
            occupied[i][0] = true;

            continue;
        }

        /*
           Find a second-level hash function
           with no collisions.
        */

        while (true) {
            a2[i] = rand() % (p - 1) + 1;
            b2[i] = rand() % p;

            T[i].assign(size, 0);
            occupied[i].assign(size, false);

            bool collision = false;

            for (int j = 0;
                 j < (int)bucket[i].size();
                 ++j) {

                int index =
                    hashFunction(
                        bucket[i][j],
                        a2[i],
                        b2[i],
                        p,
                        size
                    );

                if (occupied[i][index]) {
                    collision = true;
                    break;
                }

                occupied[i][index] = true;
                T[i][index] = bucket[i][j];
            }

            if (!collision)
                break;
        }
    }

    /*
       Construct the test set.

       Test set =
       distinct dataset elements UNION random set.
    */

    sort(randomSet.begin(), randomSet.end());

    randomSet.erase(
        unique(randomSet.begin(), randomSet.end()),
        randomSet.end()
    );

    vector<int> testSet = keys;

    testSet.insert(
        testSet.end(),
        randomSet.begin(),
        randomSet.end()
    );

    sort(testSet.begin(), testSet.end());

    testSet.erase(
        unique(testSet.begin(), testSet.end()),
        testSet.end()
    );

    /*
       Test every key.
    */

    int TP = 0;
    int TN = 0;
    int FP = 0;
    int FN = 0;

    bool correct = true;

    for (int i = 0;
         i < (int)testSet.size();
         ++i) {

        int x = testSet[i];

        /*
           First-level lookup.
        */

        int firstIndex =
            hashFunction(x, a1, b1, p, n);

        bool found = false;
        int value = 0;

        /*
           Second-level lookup.
        */

        if (!bucket[firstIndex].empty()) {

            int size =
                T[firstIndex].size();

            int secondIndex =
                hashFunction(
                    x,
                    a2[firstIndex],
                    b2[firstIndex],
                    p,
                    size
                );

            if (occupied[firstIndex][secondIndex]) {

                value =
                    T[firstIndex][secondIndex];

                if (value == x)
                    found = true;
            }
        }

        /*
           Check actual membership in the dataset.
        */

        bool actual =
            binary_search(
                keys.begin(),
                keys.end(),
                x
            );

        /*
           Print search result.
        */

        cout<<"SEARCH " << x << ": ";

        if (found)
            cout<<"FOUND " << value << '\n';
        else
            cout<<"NOT FOUND\n";

        /*
           Update confusion matrix.
        */

        if (actual && found && value == x)
            ++TP;
        else if (actual && !found)
            ++FN;
        else if (!actual && found)
            ++FP;
        else
            ++TN;

        if (actual != found ||
            (found && value != x))
            correct = false;
    }

    /*
       Print confusion matrix.
    */

    cout<<"CONFUSION_MATRIX\n";
    cout<<"ACTUAL/PREDICTED PRESENT ABSENT\n";
    cout<<"PRESENT " << TP << ' ' << FN << '\n';
    cout<<"ABSENT " << FP << ' ' << TN << '\n';

    if (correct)
        cout<<"RESULT: CORRECT\n";
    else
        cout<<"RESULT: INCORRECT\n";

    return 0;
}