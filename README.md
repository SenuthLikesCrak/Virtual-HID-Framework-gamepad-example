# Project Setup and Troubleshooting Guide

### Environment Setup
Follow this link to get your environment set up: [Microsoft WDK Setup Guide](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk)

### Common Build Issues
If you encounter any build errors, try the following steps:

1. **Add the Vhfkm Library**  
   Go to **Properties** → **Linker** → **Input** → **Additional Dependencies** and include `"Vhfkm.lib"`.

2. **Disable Spectre Mitigation**  
   Go to **Properties** → **C/C++** → **Code Generation** → **Spectre Mitigation** and set it to "No".

3. **Enable Local Time for inf2Cat**  
   Go to **Properties** for inf2Cat and turn on "Use Local Time".

### Driver Installation Tips
If you're having trouble installing the driver, check these points:

1. **Avoid Device Manager for Installation**  
   You cannot install this driver through **Device Manager**. Instead, use **devcon**, which is included in the WDK.

2. **Enable Test Signing**  
   Ensure **Test Signing** is enabled. Note that you’ll need to disable **Secure Boot** to activate Test Signing mode.

---

This guide should help you get set up and troubleshoot common issues. For further assistance, refer to the official [Microsoft Documentation](https://learn.microsoft.com/).
