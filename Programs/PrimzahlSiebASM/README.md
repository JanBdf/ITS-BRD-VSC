# Task 4

> Jan Bittendorf | 14.05.2026

- - - 

## Analyse der Aufgabe

Die Zahlen von 2 bis 1000 sollen durchgegangen werden. Für jede Zahl sollen alle Vielfachen dieser Zahl als Kandidaten für Primzahlen entfernt werden. Wenn eine Zahl schon rausgestrichem ist, kann sie übersprungen werden, da ihre Vielfachen ebenfalls schon rausgestrichen sind. 
Da für jede beliebige Zahl die kleinste, nicht rausgestrichene Zahl das Quadrat dieser ist, können alle Zahlen über 31 ignoriert werden, da ihr Quadrat über 1000 liegt.

Anschließend müssen wir nur noch diese Liste der Zahlen durchgehen und diejenigen Zahlen in ein separates Array / Feld schreiben, die noch als Primzahl geblieben sind.

## Pseudocode (in c style)

private int8_t* calcPrimes(void) {
    uint8_t num_arr[999] = {[0 ... 998] = 1}

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
    uint16_t primes[500] = {0};
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
