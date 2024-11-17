[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NRF24L01

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/nrf24l01/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

nRF24L01+는 베이스밴드 프로토콜 엔진(Enhanced ShockBurst™)이 내장된 단일 칩 2.4GHz 트랜시버로 초저전력 무선 애플리케이션에 적합합니다. nRF24L01+는 2.400 - 2.4835GHz의 전 세계 ISM 주파수 대역에서 작동하도록 설계되었습니다. nRF24L01+로 무선 시스템을 설계하려면 MCU(마이크로컨트롤러)와 몇 가지 외부 수동 부품만 있으면 됩니다. nRF24L01+를 작동 및 구성할 수 있습니다. 직렬 주변기기 인터페이스(SPI)를 통해. SPI를 통해 액세스할 수 있는 레지스터 맵은 nRF24L01+의 모든 구성 레지스터를 포함하며 칩의 모든 작동 모드에서 액세스할 수 있습니다. 임베디드 베이스밴드 프로토콜 엔진(Enhanced ShockBurst™)은 패킷 통신을 기반으로 하며 수동에서 다양한 모드를 지원합니다. 고급 자율 프로토콜 작동으로의 작업. 내부 FIFO는 무선 프런트 엔드와 시스템의 MCU 간의 원활한 데이터 흐름을 보장합니다. Enhanced Shock-Burst™는 모든 고속 링크 계층 작업을 처리하여 시스템 비용을 줄입니다. 무선 프론트 엔드는 GFSK 변조를 사용합니다. 주파수 채널, 출력 전력 및 공기 데이터 속도와 같은 사용자 구성 가능한 매개변수가 있습니다. nRF24L01+는 250kbps, 1Mbps 및 2Mbps의 무선 데이터 속도를 지원합니다. nRF24L01+는 두 가지 절전 모드와 결합된 높은 공기 데이터 속도를 통해 초저전력 설계에 매우 적합합니다. nRF24L01+는 nRF24L01과 드롭인 호환이 가능하고 nRF2401A, nRF2402, nRF24E1 및 nRF24E2와 방송 호환이 가능합니다. nRF24L01+의 상호변조 및 광대역 차단 값은 nRF24L01에 비해 훨씬 개선되었으며 nRF24L01+에 내부 필터링을 추가하여 RF 규제 표준을 충족하기 위한 마진을 개선했습니다. 내부 전압 조정기는 높은 PSRR(Power Supply Rejection Ratio) 및 넓은 전력을 보장합니다. 공급 범위. NRF24L01은 무선 PC 주변 장치, 마우스, 키보드, 리모콘, 게임 컨트롤러 등에 사용됩니다.

LibDriver NRF24L01은 LibDriver에서 출시한 NRF24L01의 전체 기능 드라이버입니다. 무선 전송, 무선 수신 등의 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic send](#example-basic-send)
    - [example basic receive](#example-basic-receive)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver NRF24L01의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver NRF24L01용 플랫폼 독립적인 SPI버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver NRF24L01드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver NRF24L01프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver NRF24L01오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 NRF24L01데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인SPI 버스 템플릿을 참조하여 지정된 플랫폼에 대한 SPI버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

```c
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

### 문서

온라인 문서: [https://www.libdriver.com/docs/nrf24l01/index.html](https://www.libdriver.com/docs/nrf24l01/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.