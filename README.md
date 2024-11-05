Follow this link to get your environment setup: https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk

If you are encountering any build errors,
  Make sure you have included the "Vhfkm.lib" in 'Properties->Linker->Input->Additional Dependencies'
  Turn off Spectre Mitigation in 'Properties->C/C++->Code Generation->Spectre Mitigation'
  Turn on "Use Local Time" in inf2Cat (Properties)

If you are having trouble installing the driver,
  You cannot install this driver through device manager, use devcon for installation (devcon is provided in the WDK)
  Make sure you have test signing on (You need to disable secure boot to turn on test signing)
