# SetupVar for Windows

SetupVar is a Windows utility for reading and writing NVRAM variables.

## Supported variables

Currently, SetupVar supports the following variables:
| Variable    | GUID | Command  |
|---------|-----|-------------|
| Setup | EC87D643-EBA4-4BB5-A1E5-00003F3E3FBB | -AmiSetup |
| Setup | A04A27F4-DF00-4D42-B552-39511302113D | -InsydeSetup |
| PchSetup | 4570B7F1-ADE8-4943-8DC3-406472842384 | -PchSetup |
| SaSetup | 72C5E28C-7783-43A1-8767-FAD73FCCAFA4 | -SaSetup |
| CpuSetup | B08F97FF-E6E8-4193-A997-5E9E9B0ADB32 | -CpuSetup |
| IntelSetup | EC87D643-EBA4-4BB5-A1E5-3F3E36B20DA9 | -IntelSetup |
| SystemConfig | A04A27F4-DF00-4D42-B552-39511302113D | -SystemConfig |
| KEK | 8BE4DF61-93CA-11D2-AA0D-00E098032B8C | -AmiKek |
| PK | 8BE4DF61-93CA-11D2-AA0D-00E098032B8C | -AmiPk |

Please note that if some of these variables are write-protected, SetupVar cannot change them.

## Syntax

To display details about the program, enter `SetupVar`:
![Syntax](https://i.imgur.com/w7WEOYT.png)

To print the data of a specific NVRAM variable, enter `SetupVar -VariableName`:
![Print data](https://i.imgur.com/dYGKPAe.png)

To edit the data of a specific NVRAM variable, enter `SetupVar -VariableName Offset Value`:
![Edit data](https://i.imgur.com/NTemgdo.png)

## Warning

Changing NVRAM variables can be risky. Incorrect changes may brick your device, rendering it unusable. Proceed with caution and ensure you understand the implications of any changes you make.
