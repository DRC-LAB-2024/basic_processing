# Smart Home Native App

This is a React Native application for managing smart home devices.

## Prerequisites

- Node.js and npm installed on your machine
- Expo CLI installed globally (`npm install -g expo-cli`)

## Installation

1. Clone the repository to your local machine.
2. Navigate to the project directory.
3. Run `npm install` to install the dependencies.

## Project Structure

```
smart_home/
├── **tests**/ # Unit tests
├── android/ # Android native code
├── ios/ # iOS native code
├── src/ # Source files
│ ├── components/ # Reusable components
│ ├── screens/  # Screen components
│ ├── redux/ # Redux state management files
│ ├── services/ # Services
│ ├── utils/ # Utility functions
├── assets/ # Assets (e.g., images, fonts)
├── modules/react-native-sql/ # custom native module (now availible on android)
│── App.js # Main application file
├── node_modules/ # Installed packages
├── .prettierrc # Prettier configuration
├── .gitignore # Specifies intentionally untracked files to ignore
├── package.json # Project dependencies and scripts
├── app.json # Expo configuration
└── README.md # Project description and instructions
```

## Running the Application

- Now, App is running on Android only.
- To run the application on IOS device/emulator, add native module for IOS in folder `modules/react-native-sql/` and run `npx expo run:ios`.
- To run the application on an Android device/emulator , run `npx expo run:android`.

## Dependencies

This project uses several dependencies, including:

- `@gluestack-style/react`, `@gluestack-ui/config`, and `@gluestack-ui/themed` for UI components and theming.
- `@react-native-async-storage/async-storage` for local data storage.
- `@react-native-community/netinfo` for network information.
- `@react-navigation/bottom-tabs`, `@react-navigation/native`, and `@react-navigation/native-stack` for navigation.
- `@reduxjs/toolkit`, `react-redux`, and `redux` for state management.
- `react-native-ble-plx` and `react-native-bluetooth-classic` for Bluetooth functionality.
- `react-native-date-picker` for date selection.
- `react-native-loading-spinner-overlay` for loading spinners.
- `react-native-safe-area-context` and `react-native-screens` for screen layout.
- `react-native-svg` and `react-native-vector-icons` for SVG and icon support.
- And others.

Please refer to the `package.json` file for the complete list of dependencies.
