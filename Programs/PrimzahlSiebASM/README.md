# Task 4

> Jan Bittendorf | 21.05.2026

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

## Konzept zur Umwandlung in Assembly

### Schleifen

Verwendet wurden zwei `for` Schleifen. Diese müssen in Assembly als while Schleife umgesetzt werden. 
Dafür setzen wir zuerst unsere Laufvariable und überprüfen, ob sie noch innerhalb des festgelegten Ramens liegt. 
Falls nicht, springen wir zum Ende. 
Als nächstes folgt der Inhalt der Schleife. 
Am Ende des Inhalts zählen wir unsere Laufvariable hoch und springen zurück zur Überprüfung.

### Vergleiche und Spruchbefehle

Einerseits wollen wir bei den Schleifen zum Ende springen, wenn wir den Schwellwert überschritten haben. Dafür können `CMP` und `BGT` verwendet werden, um zu springen, wenn der im Register gespeicherte Wert größer ist, als der Vergleichswert.
Andererseits überprüfen wir, ob Zahlen selbst Vielfache sind, um die Vielfachen nicht erneut rauszustreichen und um damit Laufzeit zu sparen. Dazu müssen überprüfen, ob der Wert im Array `0` ist. Verwendet werden können der Vergleich `CMP` und der Sprungbefehl `BEQ` um zurück zu Schleife zu springen, wenn der Wert dem Vergleichswert entspricht.

## Datentypen

Das Sieb wird angelegt, indem wir im Programmbereich `MyData` einen Speicherbereich mit den Befehl `FILL` reservieren. Dabei können wir angeben, dass der Bereich mit Einsen gefüllt werden soll. Angegeben werden muss außerdem die Größe eines Elements als 1 Byte. Danach können wir die Startadresse laden und mit einem Offset auf spezifische Elemente des Arrays zugreifen.
