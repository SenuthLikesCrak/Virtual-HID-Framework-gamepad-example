# Project Setup and Troubleshooting

### Environment Setup
Follow the instructions in the [Microsoft WDK Setup Guide](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk).

### Build Issues
If you encounter any build errors, try the following steps:

1. **Add the Vhfkm Library**  
   Navigate to **Properties** → **Linker** → **Input** → **Additional Dependencies** and add `"Vhfkm.lib"`.

2. **Disable Spectre Mitigation**  
   In **Properties** → **C/C++** → **Code Generation** → **Spectre Mitigation**, set it to "No".

3. **Enable Local Time for inf2Cat**  
   Go to **Properties** for inf2Cat and enable "Use Local Time".

### Driver Installation
If you're having trouble installing the driver:

1. **Avoid Device Manager**  
   You cannot install this driver through **Device Manager**. Instead, use **devcon**, which is provided in the WDK.

2. **Enable Test Signing**  
   Make sure **Test Signing** is enabled. You’ll need to disable **Secure Boot** to activate Test Signing mode.

---

For additional information, refer to [Write a HID source driver by using Virtual HID Framework (VHF)](https://learn.microsoft.com/en-us/windows-hardware/drivers/hid/virtual-hid-framework--vhf-).

I hope this repository helped you in someway. I wanted to share it due to the lack of examples provided by Microsoft on VHF.
