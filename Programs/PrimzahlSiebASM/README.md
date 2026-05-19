# Task 4

> Jan Bittendorf | 14.05.2026

- - - 

## Pseudocode (in c style)

private int8_t* calcPrimes(void) {
    int8_t num_arr[999] = {[0 ... 998] = 1}

    for (int16_t base = 2; base <= 31; base++) {
        if (num_arr[base - 2] == 0) {
            continue;
        }

        for (int16_t prod = base * base; prod <= 1000; prod += base) {
            num_arr[prod - 2] = 0;
        }
    }

    return num_arr;
}

private int16_t* storePrimes(int8_t *num_arr) {
    int16_t primes[500] = {0};
    num_primes = 0;

    for (int index = 0; index <= 998; index++) {
        if (num_arr[index] == 1) {
            primes[num_primes++] = index + 2;
        }
    }

    return primes;
}

public void main(void) {
    int8_t *num_arr = calcPrimes();
    int16_t *primes = storePrimes(num_arr);
}
