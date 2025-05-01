# STM32 Project – Build & Flash with VSCode + CMake

Ten projekt zawiera konfigurację środowiska dla STM32 (STM32F429ZI) opartą o `STM32CubeMX`, `CMake`, `arm-none-eabi` toolchain oraz `OpenOCD`, z pełną integracją z Visual Studio Code.

---

## Wymagania wstępne
### Zainstaluj toolchain ARM
Pobierz ze strony [ARM GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads). Przykład:

```bash
cd ~/Downloads
wget https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz
tar -xf arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz -C ~/.local/
```

Dodaj do `.bashrc` lub `.zshrc`:

```bash
export PATH="$HOME/.local/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi/bin:$PATH"
```

Zastosuj:
```bash
source ~/.bashrc
```

Sprawdź:
```bash
arm-none-eabi-gcc --version
```

---

## Zainstaluj OpenOCD
```bash
sudo apt install openocd
```

Sprawdź:
```bash
openocd --version
```

---

## Konfiguracja Visual Studio Code

### 1. Zainstaluj rozszerzenia:

- [Cortex-Debug](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug)
- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- [statusbar-commands](https://marketplace.visualstudio.com/items?itemName=anweber.statusbar-commands)

---

## Taski VSCode

W katalogu `.vscode/` stwórz plik `tasks.json`:

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Configure and Build",
      "type": "shell",
      "command": "mkdir -p build && cd build && cmake .. && make -Bj",
      "problemMatcher": [],
      "detail": "Create build dir, configure CMake and build"
    },
    {
      "label": "Flash",
      "type": "shell",
      "command": "openocd",
      "args": [
        "-f", "interface/stlink.cfg",
        "-f", "target/stm32f4x.cfg",
        "-c", "program build/SR-edge-detection.elf verify reset exit"
      ],
      "problemMatcher": [],
      "detail": "Flash firmware to STM32 using OpenOCD"
    },
    {
      "label": "Build and Flash",
      "type": "shell",
      "command": "mkdir -p build && cd build && cmake .. && make -Bj && cd .. && openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c \"program build/SR-edge-detection.elf verify reset exit\"",
      "problemMatcher": [],
      "group": {
        "kind": "build",
        "isDefault": false
      },
      "detail": "Build firmware and flash to STM32"
    }
  ]
}
```

---

## Plik `launch.json` do debugowania (w `.vscode/`)

```json
{
    "version": "0.2.0",
    "configurations": [
      {
        "name": "Debug STM32",
        "type": "cortex-debug",
        "request": "launch",
        "servertype": "openocd",
        "cwd": "${workspaceFolder}",
        "executable": "${workspaceFolder}/build/SR-edge-detection.elf",
        "device": "STM32F429ZI",  
        "configFiles": [
          "interface/stlink.cfg",
          "target/stm32f4x.cfg"
        ],
        "runToEntryPoint": "main",
      }
    ]
}

```

---

## Przyciski do paska statusu (statusbar-commands)

Dodaj do `settings.json`:

```json
"statusbar_command.commands": [
  {
    "text": "⚙️ Build",
    "tooltip": "Build firmware",
    "command": "workbench.action.tasks.runTask",
    "arguments": ["Configure and Build"],
    "alignment": "left",
    "priority": 1
  },
  {
    "text": "🚀 Flash",
    "tooltip": "Flash firmware to STM32",
    "command": "workbench.action.tasks.runTask",
    "arguments": ["Flash"],
    "alignment": "left",
    "priority": 2
  },
  {
    "text": "🔁 Build & Flash",
    "tooltip": "Build and flash to STM32",
    "command": "workbench.action.tasks.runTask",
    "arguments": ["Build and Flash"],
    "alignment": "left",
    "priority": 3
  }
]
```
---

## Uruchamianie projektu

Po wykonaniu powyższych kroków:

1. Kliknij `🔁 Build & Flash` na dolnym pasku
2. Naciśnij `F5`, aby rozpocząć debugowanie

---