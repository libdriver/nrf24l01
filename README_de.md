[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NRF24L01

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/nrf24l01/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der nRF24L01+ ist ein Single-Chip-2,4-GHz-Transceiver mit einer eingebetteten Basisband-Protokoll-Engine (Enhanced ShockBurst™), der für drahtlose Anwendungen mit extrem geringem Stromverbrauch geeignet ist. Der nRF24L01+ ist für den Betrieb im weltweiten ISM-Frequenzband bei 2,400 - 2,4835 GHz ausgelegt. Um ein Funksystem mit dem nRF24L01+ zu entwerfen, benötigen Sie lediglich eine MCU (Mikrocontroller) und einige externe passive Komponenten. Sie können den nRF24L01+ bedienen und konfigurieren über ein Serial Peripheral Interface (SPI). Die über das SPI zugängliche Registerkarte enthält alle Konfigurationsregister im nRF24L01+ und ist in allen Betriebsmodi des Chips zugänglich Betrieb zum fortgeschrittenen autonomen Protokollbetrieb. Interne FIFOs sorgen für einen reibungslosen Datenfluss zwischen dem Funk-Frontend und der MCU des Systems. Enhanced Shock-Burst™ reduziert die Systemkosten, indem es alle Hochgeschwindigkeitsoperationen der Verbindungsschicht handhabt. Das Funk-Front-End verwendet GFSK-Modulation. Es verfügt über vom Benutzer konfigurierbare Parameter wie Frequenzkanal, Ausgangsleistung und Luftdatenrate. nRF24L01+ unterstützt eine Luftdatenrate von 250 kbps, 1 Mbps und 2 Mbps. Die hohe Luftdatenrate in Kombination mit zwei Energiesparmodi machen den nRF24L01+ sehr geeignet für Designs mit extrem niedrigem Stromverbrauch. Der nRF24L01+ ist drop-in-kompatibel mit nRF24L01 und on-air-kompatibel mit nRF2401A, nRF2402, nRF24E1 und nRF24E2. Die Intermodulations- und Breitbandblockierwerte des nRF24L01+ sind im Vergleich zum nRF24L01 deutlich verbessert, und das Hinzufügen einer internen Filterung zum nRF24L01+ hat die Margen zur Erfüllung der HF-Regulierungsstandards verbessert. Interne Spannungsregler gewährleisten ein hohes Power Supply Rejection Ratio (PSRR) und eine breite Leistung Lieferprogramm. NRF24L01 wird in drahtlosen PC-Peripheriegeräten, Mäusen, Tastaturen, Fernbedienungen, Gamecontrollern usw. verwendet.

LibDriver NRF24L01 ist der Treiber mit vollem Funktionsumfang von NRF24L01, der von LibDriver gestartet wurde. Er bietet Funktionen für drahtloses Senden, drahtloses Empfangen usw. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic send](#example-basic-send)
    - [example basic receive](#example-basic-receive)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver NRF24L01-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver NRF24L01 SPI.

/test enthält den Testcode des LibDriver NRF24L01-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver NRF24L01-Beispielcode.

/doc enthält das LibDriver NRF24L01-Offlinedokument.

/Datenblatt enthält NRF24L01-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige SPI-Schnittstellenvorlage und stellen Sie Ihren Plattform-SPI-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

#### example basic send

```C
#include "driver_nrf24l01_basic.h"

uint8_t (*g_gpio_irq)(void) = NULL;
uint8_t res;
uint8_t addr[5] = NRF24L01_BASIC_DEFAULT_RX_ADDR_0;

...

static void a_callback(uint8_t type, uint8_t num, uint8_t *buf, uint8_t len)
{
    switch (type)
    {
        case NRF24L01_INTERRUPT_RX_DR :
        {
            uint8_t i;
            
            nrf24l01_interface_debug_print("nrf24l01: irq receive with pipe %d with %d.\n", num, len);
            for (i = 0; i < len; i++)
            {
                nrf24l01_interface_debug_print("0x%02X ", buf[i]);
            }
            nrf24l01_interface_debug_print(".\n");
            
            break;
        }
        case NRF24L01_INTERRUPT_TX_DS :
        {
            nrf24l01_interface_debug_print("nrf24l01: irq send ok.\n");
            
            break;
        }
        case NRF24L01_INTERRUPT_MAX_RT :
        {
            nrf24l01_interface_debug_print("nrf24l01: irq reach max retry times.\n");
            
            break;
        }
        case NRF24L01_INTERRUPT_TX_FULL :
        {
            break;
        }
        default :
        {
            break;
        }
    }
}

...

res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}
g_gpio_irq = nrf24l01_interrupt_irq_handler;
res = nrf24l01_basic_init(NRF24L01_TYPE_TX, a_callback);
if (res != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}
if (nrf24l01_basic_send((uint8_t *)addr, (uint8_t *)"123", 3) != 0);
{
    (void)nrf24l01_basic_deinit();
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;
    
    return 1;
}
if (nrf24l01_basic_deinit() != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;
    
    return 1;
}

(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;

return 0;
```

#### example basic receive

```C
#include "driver_nrf24l01_basic.h"

uint8_t (*g_gpio_irq)(void) = NULL;
uint8_t res;
uint32_t timeout;

...

static void a_callback(uint8_t type, uint8_t num, uint8_t *buf, uint8_t len)
{
    switch (type)
    {
        case NRF24L01_INTERRUPT_RX_DR :
        {
            uint8_t i;
            

            nrf24l01_interface_debug_print("nrf24l01: irq receive with pipe %d with %d.\n", num, len);
            for (i = 0; i < len; i++)
            {
                nrf24l01_interface_debug_print("0x%02X ", buf[i]);
            }
            nrf24l01_interface_debug_print(".\n");
            
            break;
        }
        case NRF24L01_INTERRUPT_TX_DS :
        {
            nrf24l01_interface_debug_print("nrf24l01: irq send ok.\n");
            
            break;
        }
        case NRF24L01_INTERRUPT_MAX_RT :
        {
            nrf24l01_interface_debug_print("nrf24l01: irq reach max retry times.\n");
            
            break;
        }
        case NRF24L01_INTERRUPT_TX_FULL :
        {
            break;
        }
        default :
        {
            break;
        }
    }

}

...

timeout = 5000;
res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}
g_gpio_irq = nrf24l01_interrupt_irq_handler;
res = nrf24l01_basic_init(NRF24L01_TYPE_RX, a_callback);
if (res != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}
nrf24l01_interface_delay_ms(timeout);
if (nrf24l01_basic_deinit() != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;
    
    return 1;
}

(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;

return 0;
```

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/nrf24l01/index.html](https://www.libdriver.com/docs/nrf24l01/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.