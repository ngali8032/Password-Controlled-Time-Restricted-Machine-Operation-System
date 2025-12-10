TimeGuard Access Shield – Password-Controlled Time-Restricted Machine Operation System
Overview:
TimeGuard Access Shield is a secure, password-controlled machine operation system designed to
restrict device usage to predefined working hours.
Hardware Requirements:
- Microcontroller: LPC2148
- 16×2 LCD Display
- 4×4 Matrix Keypad
- External Interrupt Switch
- Relay for Machine Control
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
RTC Editing:
Options: Hour, Minute, Second, Day, Date, Month, Year, Exit
Validation includes hour (0–23), minute/second (0–59), date rules, etc.
Allowed Time Editing:
Set Start Time, Set End Time, Exit
Validation: HH:MM:SS format
Password Change:
Enter current password → Enter new password → Confirm new password
Exit:
Returns to main operation loop.
Conclusion:
This system uses RTC-based timing control, secure password checks, and interrupt-driven
configuration.
