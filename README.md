# kTerminal

## Command list
| **System Command**                  |                                     |
| :---------------------------------- | ----------------------------------- |
| -HELP                               | show command list                   |
| -INFO                               | show configuration                  |
| -VERSION                            | show firmware version               |
| -AUTO                               | select available port automatically |
| -PORT LIST                          | select serial comport with list     |
| -PORT [PORT/COMx]                   | serial comport setting              |
| -PORT [PORT/COMx] [BAUDRATE]        | serial comport and baudrate setting |
| -BAUD LIST                          | select baudrate with list           |
| -BAUD [BAUDRATE]                    | serial baudrate setting             |
| -TERMINAL                           | uart terminal                       |
| **Target Command**                  |                                     |
| -TATGET CHECK                       | check device                        |
| -TATGET AUTO                        | set baudrate automatic              |
| -TATGET BAUD [BAUD]                 | set target baudrate                 |
| -TATGET RATE [RATE]                 | set target update rate              |
| -TATGET MODE [MODE]                 | set target mode                     |
| -TATGET GET [ITEM]                  | get baud, rate, id, mode ...        |
| **I2C Command**                     |                                     |
| -SCAN                               | i2c scan device                     |
| -REG [ADDRESS]                      | i2c scan register data              |
| -READ [ADDRESS] [REGISTER]          | i2c single read data                |
| -READ [ADDRESS] [REGISTER] [LENS]   | i2c multiple read data              |
| -WRITE [ADDRESS] [REGISTER] [DATA]  | i2c single/multiple write data      |
  
## kTerminal device program based on NRF52833-DK
https://github.com/Hom-Wang/NRF52833-DK_kTerminal
