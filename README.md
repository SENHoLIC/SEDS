#README

# Task 1: Finding the Sea Floor

Used Python to process the provided depth sensor CSV data.

## Approach

- Loaded and extracted the depth data using Pandas.
- Generated a time axis assuming one reading per second.
- Identified and handled noisy/abnormal readings.
- Plotted the processed depth against time using Matplotlib.

This helps visualize changes in depth and identifies potentially dangerous shallow regions.

---

# Task 2: Athena's Intern

Built an Arduino monitoring system in Tinkercad using a light sensor, ultrasonic sensor, push button, LCD, LED, and buzzer.

## Approach

- Used an LDR reading below `512` to detect **STORM**, with a blinking LED as the warning.
- Used an ultrasonic sensor to detect **CHARYBDIS** when an object is closer than `100 cm`, activating the buzzer.
- Used a push button to toggle the **ANCHOR DROPPED** state, protecting the ship from hazards.
- Implemented a state machine using `enum` and `switch` to manage **OPEN SEA**, **ANCHOR DROPPED**, **STORM**, **CHARYBDIS**, and **WRECKED**.
- Used `millis()` to track continuous danger and transition to **WRECKED** after `5 seconds`.
- Displayed the current state on the LCD.

This allows the system to continuously monitor hazards, respond to user input, and manage the ship's state automatically.

ThinkerCad Link: https://www.tinkercad.com/things/lly7G3LiDrp-athenas-intern-?sharecode=9iONrZv39ifyNwDU_UJ-KCoIgD_1QDnR-sug99MYklI
