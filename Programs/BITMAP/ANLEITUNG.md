<style>
  p {
    text-align: justify;
    hyphens: auto;
  }
  h1, h2, h3, h4, h5, h6 {
    text-align: left !important;
  }
  @media print {
    body {
      margin: 20mm 20mm 20mm 20mm !important; /* Oben, Rechts, Unten, Links */
    }
  }
</style>
# BITMAP_Starthilfe – Anleitung für Studierende
Sie erhalten für die Übung BITMAP **einige C-Quell- und Header-Dateien** und ein kleines Demo-Programm – als Starthilfe. Die CMSIS-Umgebung für Ihre Lösung legen Sie selbst an (Abschnitt 2) und stellen die nötigen Einstellungen ein (Abschnitt 4). Abschnitt 5 beschreibt das Verhalten des Demo-Programms auf dem Board.

## 1. Umfang der bereitgestellten Dateien
Die mitgelieferten C-Quell- und Header-Dateien sollten Sie nicht verändern, ansonsten wird ggf. das Protokoll zwischen der PC-Anwendung, die die Bilder an das ITS-Board schickt, und dem Code auf dem ITS-Board gestört. Diese Software ist relativ neu – falls Sie Probleme in der Software vermuten, schicken Sie mir eine Benachrichtigung mit dem Testfall. `main.c` ist der Einstieg für Ihre Programmierung.
Die Abhängigkeiten der Module untereinander sind als UML-ähnliches Diagramm in `Docs/ModulAbhaengigkeiten.svg` dargestellt (Quelle: `Docs/ModulAbhaengigkeiten.md`).

Hier die Dateien für den `Src/`-Ordner:

```
main.c                 Rahmenprogramm der Starthilfe
input.c                Protokoll-/Dateischicht: ask_transport, init_input (Handshake H/R/G),
                       open_nxt_file, nxt_chr, read_file, eof, wait_for_s_i, show_s5_hint
byte_io.c              Transport-Dispatcher (wählt Backend zur Laufzeit)
byte_io_usb.c          USB-CDC-Backend (Ringpuffer + Endpoint-Arm)
byte_io_uart.c         UART-via-USB-Backend (USART3 + DMA)
ringbuf.c              Empfangs-Ringpuffer (USB-ISR -> Consumer)
headers.c              BITMAP-Header einlesen + prüfen (read_hdrs, get_file_hdr, get_info_hdr)
fileHeader.c           input_file_header(): Datei öffnen, Header prüfen, auf LCD ausgeben
errorhandler.c         Fehlerbehandlung (CHECK, CHECK_FATAL, RETURN_ON_ERROR, ...)
usb_device.c           USB-Device-Stack starten (usb_device_init)
usb_clock_config.c     Takt auf 168 MHz umstellen (USB braucht Vielfaches von 42 MHz)
usbd_cdc_if.c          CDC-Anwendungsadapter (fops, Hooks cdc_collect/cdc_transmit/...)
usbd_conf.c            HAL-Glue (USBD_static_malloc/free, ...)
usbd_desc.c            USB-Deskriptoren (VID/PID, Strings)
```

`Inc/` – die zugehörigen Header (`input.h`, `byte_io.h`, `byte_io_backend.h`,
`cdc_input_if.h`, `ringbuf.h`, `transport.h`, `headers.h`, `fileHeader.h`,
`errorhandler.h`, `BMP_types.h`, `MS_basetypes.h`, `usb_device.h`,
`usb_clock_config.h`, `usbd_cdc_if.h`, `usbd_conf.h`, `usbd_desc.h`).

### Welche Funktionen aus welchen Modulen Sie für die Lösung brauchen

Für die Aufgabe in `main.c` (Rest der Datei einlesen, Farbtabelle und Pixeldaten dekodieren, auf dem LCD ausgeben) genügen die folgenden Funktionen der vorgegebenen Module:

- `input.h` (`input.c`) – Lesezugriff auf die übertragene Datei: `read_file(buf, size, count)` (blockweises Lesen von Farbtabelle und Pixeldaten), `nxt_chr()` (einzelnes Byte, z. B. RLE8-Steuerbytes), `eof()` (Dateiende abfragen, ohne ein Byte zu konsumieren), `open_nxt_file()` (nächste Datei anfordern) und `wait_for_s_i(BUTTON_S5)` (auf S5/PF5 warten).
- `headers.h` (`headers.c`) – nach `input_file_header()` (ruft intern bereits `read_hdrs()` auf): `get_file_hdr(&file_hdr)` und `get_info_hdr(&info_hdr)` liefern die geprüften Header (u. a. `bfOffBits`, `biWidth`, `biHeight`, `biBitCount`, `biCompression`, `biClrUsed`).
- `fileHeader.h` (`fileHeader.c`) – `input_file_header()` kapselt `open_nxt_file()` + `read_hdrs()` + LCD-Ausgabe beider Header + Warten auf S5; läuft bereits in der Schleife in `main.c`.
- `BMP_types.h` – Typen und Konstanten des BMP-Formats: `BITMAPFILEHEADER`, `BITMAPINFOHEADER`, `RGBQUAD` (Farbtabelleintrag bei 8 Bit Farbtiefe), `RGBTRIPLE` (Pixel bei 24 Bit Farbtiefe), `BI_RGB`, `BI_RLE8`, `MAX_COLOR_TABLE_SIZE`.
- `errorhandler.h` (`errorhandler.c`) – Fehlerbehandlung: Rückgabewert `EOK` und die Fehlercodes (`READ_ERR`, `HEADER_ERR`, `INV_HEADER_ERR`, `COL_TAB_ERR`, `UNSUPPORTED_FORMAT_ERR`, `FORMAT_ERR`, `IMG_SIZE_ERR`, ...) sowie die Makros `CHECK`, `CHECK_FATAL`, `CHECK_OR_RETURN`, `RETURN_ON_ERROR`.

Für die Ausgabe auf dem LCD werden keine BITMAP-Module benötigt, sondern die Bibliotheken des Repos: `LCD_GUI.h` (DisplayWaveshare) mit `GUI_clear()`, `GUI_drawPoint()` für einzelne Pixel und `GUI_WriteLine()` für zeilenweise Ausgabe (die Musterlösung puffert eine ganze Zeile, bevor sie sie schreibt), `LCD_general.h` für den Farbtyp `COLOR` (RGB565) und die Display-Abmessungen (`LCD_WIDTH`/`LCD_HEIGHT`), sowie `lcd.h` (ITS_BRD_LIB) mit `lcdPrintS()`, `lcdPrintlnS()`, `lcdPrintInt()`, `lcdGotoXY()` für Textausgaben.

## 2. Neues CMSIS-Projekt anlegen
Wie üblich legen Sie über `./createNewProjectC.sh` (macOS/Linux) bzw. `createNewProjectC.bat` (Windows) – aufgerufen aus dem Ordner `bin` – den Rahmen für ein neues CMSIS-Programm an. Wählen Sie wie üblich das Projekt als neue CMSIS Solution aus (`Programs/<Name>/<Name>`), machen Sie einen Refresh sowie einen `Update Tool registry` durch Rechtsklick auf die Datei `vcpkg-configuration.json` (Details siehe `README.md` im Wurzelverzeichnis, Abschnitt „Neues Projekt erstellen“).

## 3. Bereitgestellte Dateien übernehmen
- alle `*.c`-Dateien nach `Programs/<Name>/Src/` kopieren
- alle `*.h`-Dateien nach `Programs/<Name>/Inc/` kopieren

## 4. Einstellungen im neuen Projekt
Arbeiten Sie diese Kapitel sorgfältig durch – hier werden wesentliche Einstellungen durchgeführt, die Sie eventuell zum ersten Mal machen.

### 4.1 `<Name>.cproject.yml` – Include-Pfade der USB-Middleware
Unter `setups` -> `Options for ITSboard` -> `add-path` zwei Einträge ergänzen (der Pfad `Inc` ist bereits vorhanden und nimmt die kopierten Header auf):
```yaml
      add-path:
        - Inc
        # ... die vorhandenen Pfade ...
        - ../../stm32cubef4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
        - ../../stm32cubef4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
```

### 4.2 `<Name>.cproject.yml` – Define für USB (PCD) ergänzen
Unter `define` einen Eintrag ergänzen:

```yaml
      define:
        # ... die vorhandenen Defines ...
        - HAL_PCD_MODULE_ENABLED
```

### 4.3 `<Name>.cproject.yml` – Gruppe `Middleware/USB` anlegen
Neue Gruppe (z. B. nach `Drivers/STM32F4xx_HAL_Driver`) mit den USB-Modulen der STM32-Middleware und den passenden HAL-Treibern:

```yaml
    - group: Middleware/USB
      files:
        - file: ../../stm32cubef4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
        - file: ../../stm32cubef4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
        - file: ../../stm32cubef4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
        - file: ../../stm32cubef4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c
        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c
        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c
```

### 4.4 `<Name>.cproject.yml` – eigene C-Module in `Program/User/Src` eintragen
Jede übernommene `*.c`-Datei in der Gruppe `Program/User/Src` registrieren:

```yaml
    - group: Program/User/Src
      files:
        - file: Src/main.c
        - file: Src/input.c
        - file: Src/byte_io.c
        - file: Src/byte_io_usb.c
        - file: Src/byte_io_uart.c
        - file: Src/ringbuf.c
        - file: Src/headers.c
        - file: Src/fileHeader.c
        - file: Src/errorhandler.c
        - file: Src/usb_device.c
        - file: Src/usb_clock_config.c
        - file: Src/usbd_cdc_if.c
        - file: Src/usbd_conf.c
        - file: Src/usbd_desc.c
        # ... weitere eigene Dateien der Aufgabe ...
```

Die Header in `Inc/` werden über den Include-Pfad `Inc` gefunden und müssen nicht als Dateien eingetragen werden. Weitere eigene Dateien (Ihre Lösung der Aufgabe) kommen ebenfalls in `Src/` und werden hier dazugetragen.
Bauen und Übertragen: CMSIS-Symbol -> Hammer (`Build Solution`), danach Pfeil (`Load & Run`) bzw. Käfer (`Load & Debug`) – siehe `README.md` im Wurzelverzeichnis, Abschnitt „Building“.

### 4.5 Compiler-Warnungen in der USB-Middleware (harmlos)
Beim Übersetzen erzeugt der Compiler in der von ST gelieferten USB-Middleware vier Warnungen, z. B. `expression result unused` (`-Wunused-value`) in `usbd_ctlreq.c`, `missing field 'GetUsrStrDescriptor' initializer` in `usbd_cdc.c` sowie `unused parameter` in `usbd_conf.c` (`hpcd` in `HAL_PCD_MspInit`/`HAL_PCD_MspDeInit`, `size` in `USBD_static_malloc`). Diese Meldungen betreffen ausschließlich den Middleware-Code von ST, sind **nicht relevant** und können ignoriert werden – das Programm baut trotzdem fehlerfrei und läuft einwandfrei. Achten Sie dagegen darauf, dass **Ihre eigenen Dateien** warnungsfrei kompilieren.

## 5. Verhalten des Demo-Programms
Ablauf auf dem ITS-Board nach dem Reset (Starthilfe, `main.c`):
1. Initialisierung: Board (`initITSboard()`), Takt auf 168 MHz (`switch_to_168_MHz()`, USB braucht ein Vielfaches von 42 MHz), Timer, LCD/Touch (`GUI_init`, `TP_Init`), Font-Version prüfen.
2. Transport-Auswahl auf dem LCD: `Übertragung wählen:` – **S7 (PF7) = USB-CDC**, **S6 (PF6) = UART-via-USB**. Nach der Wahl meldet sich das Board beim PC-Programm per Handshake an (`init_input()`: PC sendet `H`, Board antwortet `R`, PC sendet `G`).
   Es gibt also **zwei Kommunikationswege** zwischen PC und Board: Der **USB-CDC-Weg (S7) ist deutlich schneller** – dafür wird ein **weiteres USB-Kabel** benötigt, das den PC mit dem OTG_FS-Anschluss des Boards (CN13) verbindet; das Board erscheint am PC als virtueller COM-Port (USB Full-Speed Bulk-Transfer). Der **UART-via-USB-Weg (S6)** läuft dagegen über die USART3-Schnittstelle des Boards (115200 8N1, am PC per USB-UART-Adapter als virtueller COM-Port) und ist wegen der niedrigen Baudrate und der Request/Response-Steuerung pro Burst nur eine **sehr langsame Notlösung für den Fall**, falls der USB-CDC-Weg nicht zur Verfügung steht.
3. Danach erscheint der Hinweis `S5 (PF5) = weiter / nächstes Bild`. Erst ein Tastendruck auf **S5 (PF5)** startet die Übertragung des ersten Bilds (ein Tastendruck zählt genau einmal, egal wie lange gedrückt).
4. Pro Bild (Endlosschleife in `main.c` -> `input_file_header()`):
   - nächste Datei wird vom PC angefordert (`open_nxt_file()`),
   - BITMAP-Header (`BITMAPFILEHEADER` + `BITMAPINFOHEADER`) werden eingelesen und geprüft (`read_hdrs()`) und vollständig auf dem LCD ausgegeben (alle Felder beider Header),
   - danach wartet das Programm erneut auf **S5 (PF5)**, löscht dann das Display und beginnt mit der nächsten Datei.
   - Tritt ein Fehler auf, wird die Fehlermeldung auf dem LCD angezeigt; erst **S5 (PF5)** fordert die nächste Datei an.

**Hinweis zur Wartezeit bei großen Dateien:** Beim USB-CDC-Transport (S7) kann das Board die Übertragung nicht anhalten — der PC streamt die Datei nach dem Start der Übertragung vollständig durch, erst danach fordert das Board die nächste Datei an (`open_nxt_file()`). Wird also der Header einer großen Datei angezeigt, dauert es entsprechend lange, bis der Header des nächsten Bilds erscheint. Beim UART-via-USB-Transport (S6) ist das nicht der Fall.
Das im Info-Header auf dem LCD angezeigte Feld `biSizeImage` entspricht dabei der Größe der Bilddaten — bei nicht komprimierten BMPs also praktisch der Dateigröße; daran lässt sich abschätzen, wie lange der USB-CDC-Stream für die jeweilige Datei läuft.

**PC-Seite:** Auf dem PC muss das mitgelieferte Python-Programm laufen, das die BMP-Dateien über den virtuellen COM-Port an das Board schickt. Zur Bedienung dieses Programms siehe die Beschreibung, die dem Programm beiliegt. Pro Datei sendet das Board `F` (USB-CDC: der PC streamt danach blind, Flusskontrolle per USB-NAK) bzw. `S` (UART-via-USB: der PC antwortet pro Burst auf `D`); die Daten kommen als Bursts mit 2-Byte-Längenheader (max. 512 Bytes, Länge 0 = Dateiende).
