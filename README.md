# Cardputer-ADV-Samsung-Remote (v0.6)
Cardputer ADV Samsung Remote v.6
# Cardputer ADV Samsung IR Remote (v0.6)

A custom Samsung Smart TV infrared remote app for the **M5Stack Cardputer ADV** running on Linux Mint / Arduino IDE. Generates 38kHz native 32-bit Samsung IR protocol signals over **GPIO 44** using `IRremoteESP8266`. Fully compatible with M5Launcher!

## Features
- **Direct Hotkeys:** Press direct keys (`P`, `-`, `=`, `0`, `[`, `]`, `I`, `\`, `B`, `M`, etc.) for instant IR transmission.
- **On-Screen Grid Navigation:** Move the selection box around the 5x3 screen using `E`, `S`, `A`, `D` without firing IR.
- **Smart TV Arrow Controls:** Dedicated directional shortcuts (`;`, `.`, `,`, `/`) for navigating Netflix, YouTube, and Samsung TV menus.
- **Help & Credits Screen:** Press `H` to view on-screen key mappings and project credits.
- **Display Dimming:** Press `Fn` to cycle screen brightness (100% / 30% / 5%) to save battery.

## Keyboard Layout & Keymap
| Function | On-Screen Label | Direct Shortcut Key |
| :--- | :--- | :--- |
| **Power Toggle** | `[P] PWR` | `P` |
| **Volume - / +** | `[-] VOL-` / `[=] VOL+` | `-` / `=` |
| **Mute** | `[0] MUTE` | `0` |
| **Channel - / +** | `[[] CH-` / `[]] CH+` | `[` / `]` |
| **Input Source** | `[I] INPUT` | `I` |
| **Smart TV Navigation** | `[;] UP`, `[.] DOWN`, `[,] LEFT`, `[/] RIGHT` | `;`, `.`, `,`, `/` |
| **Exit / Back / Menu** | `[\] EXIT`, `[B] BACK`, `[M] MENU` | `\`, `B`, `M` |
| **Enter / Select** | `[SPC] OK` | `Space` / `Enter` / `GO Button` |

## Controls & Utilities
- **`E` `S` `A` `D`**: Navigate cursor on screen without firing IR.
- **`GO` / `ENTER`**: Transmit currently highlighted button.
- **`H`**: Open / Close On-Screen Help & Credits pages.
- **`Fn`**: Toggle Display Brightness (High / Dim / Stealth).

## Hardware & Environment
- **Device:** M5Stack Cardputer ADV
- **IR Pin:** GPIO 44
- **Libraries Required:** `M5Cardputer`, `M5Unified`, `IRremoteESP8266`

## Credits
- Created by **izzthatright** (with AI support)
- Inspired by **VolosR** (*M5CardRemote*) and **geo_tp** (*Ultimate Remote*)
