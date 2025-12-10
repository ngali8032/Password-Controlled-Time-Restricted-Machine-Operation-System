TimeGuard Access Shield – Password-Controlled Time-Restricted Machine Operation System
Overview:

TimeGuard Access Shield is a secure, password-controlled machine operation system designed to
restrict device usage to predefined working hours while ensuring that only authenticated users can access the machine.

Hardware Requirements:
- Microcontroller: LPC2148
- 16×2 LCD Display
- 4×4 Matrix Keypad
- External Interrupt Switch
  
Port Pin Configuration:
External Interrupt Switch: PORT0 - P0.1
4×4 Keypad: PORT1 - P1.16 to P1.23
16×2 LCD: PORT0 - P0.8 to P0.18

Project Workflow:
1. Initialize LCD, RTC, keypad, and relay.
2. Display real-time clock continuously.
3. Authenticate user with password.
4. Check allowed working hours.
5. Grant or deny access accordingly.
   
Configuration Mode (Triggered by External Interrupt P0.1):
Menu:
1. E.RTCINFO
2. E.DEV.A.TIME
3. PWD CHG
4. EXIT
   
1. RTC Information Editing (E.RTCINFO)
The user can edit Hour, Minute, Second, Day, Date, Month, and Year.
Validation Rules:
• Hour: 0–23
• Minute/Second: 0–59
• Day: 1–7
• Month: 1–12
• Date: Valid according to month rules and leap year calculations
Invalid inputs generate an error and require re-entry.

2. Device Allowed Time Editing (E.DEV.A.TIME)
User enters time in HH:MM:SS format.
Invalid inputs trigger an error and require re-entry.
Valid values are saved automatically.

3. Password Change (PWD CHG)
1. Enter current password.
2. Enter new password.
3. Re-enter new password to confirm.
4. If both match → password updated.
5. If mismatch → error displayed → retry.
   
4. Exit (EXIT)
Selecting EXIT returns control to the main logic.
All updated settings (RTC, password, allowed time) are applied immediately.

Conclusion
The TimeGuard Access Shield provides a secure, flexible, time-regulated machine operation system
featuring:
• RTC-based timing
• Password-controlled access
• Interrupt-driven configuration
This system is ideal for laboratories, industrial workshops, and environments requiring controlled
operations.
