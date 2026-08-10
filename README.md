# Roséverse Installer
<img width="1000" height="250" alt="Roséverse Installer by Project Rosé (README banner)" src="https://github.com/user-attachments/assets/240278dc-a46e-409d-9d3f-2a1bd465d644" />

[![GitHub release](https://img.shields.io/github/release/Project-Rose/Roseverse-Installer.svg)](https://github.com/Project-Rose/Roseverse-Installer/releases) [![Github all releases](https://img.shields.io/github/downloads/Project-Rose/Roseverse-Installer/total.svg)](https://gitHub.com/Project-Rose/Roseverse-Installer/releases) [![GitHub License](https://img.shields.io/github/license/Project-Rose/Roseverse-Installer)](https://github.com/Project-Rose/Roseverse-Installer/blob/master/LICENSE) [![Discord server](https://img.shields.io/discord/1277130014357327873?color=5865f2&label=Discord)](https://discord.gg/AaTsXndGun) [![Services Health](https://projectrose.montastic.io/badge)](https://projectrose.montastic.io) 

Roséverse Installer is the easiest way to get connected to [Roséverse](https://miiverse.projectrose.cafe/), Project Rose's Miiverse revival. This WUHB application lets you install and update [Inkay (Roséverse)](https://github.com/Project-Rose/Inkay-Roseverse) and [Inkay](https://github.com/PretendoNetwork/Inkay) (Pretendo version), making it easy to switch back to or update either plugin when you *absolutely* need to (We don't recommend switching frequently; please read why below).

> [!NOTE]
> This application is for the Wii U, not 3DS.

# Getting Started
## Requirements
Roséverse Installer (mainly Inkay (Roséverse))  requires a homebrewed Wii U running the latest version of the [Aroma](https://github.com/wiiu-env/Aroma) environment, configured for coldboot (autoboot). Other configurations (specifically lacking coldboot) may cause issues with SpotPass connections to Pretendo's servers. Older homebrew environments (such as Tirumissu, Haxchi, etc.) and hackless methods aren't supported. If you haven't already, homebrew your Wii U using [this guide](https://wiiu.hacks.guide/)!

## Installation
<!-- 

//////////////////////////
//// Thank you HBAS! ////
////////////////////////

### Homebrew App Store
The easiest way to install Roséverse Installer is to download it from the [HB App Store](https://hb-app.store/) directly on the console!

[<img width="1080" height="300" alt="Get it on the HB App Store (shortcut link to the HB App Store page)" src="https://github.com/user-attachments/assets/b665a650-9f9b-4db0-a980-c326a1da74b5" style="width:35%; height:auto;" />](https://hb-app.store/wiiu/Roseverse-Installer)

### Manual Installation
-->
This installation process requires a computer or any other device that can read SD cards. You can alternatively use FTP using the [FTPiiU plugin](https://github.com/wiiu-env/ftpiiu_plugin) and [FileZilla](https://filezilla-project.org). The process below only covers the computer method. Please follow these steps very closely:

1. Turn off your Wii U and eject the SD card. Insert the SD card into your device.
2. Download the [latest release](https://github.com/Project-Rose/Roseverse-Installer/releases) of `roseverse_installer.wuhb`.
3. Place `roseverse_installer.wuhb` in the Wii U Apps folder on your SD card (usually `sd:/wiiu/apps`).
4. Eject the SD card from your device, and insert the SD card into your Wii U and turn it on. 

Like all other Aroma apps, the Roséverse Installer app icon will appear on the HOME menu once it is successfully installed.

# Configuration
Click on Roséverse Installer, then wait for it to load. Once you're there, you will see 3 options that you can click to do different actions:
- The "Juxtaposition" option will install/replace the current installed plugin with [the latest version of Inkay (Pretendo version)](https://github.com/PretendoNetwork/Inkay/releases).
- The "Roséverse" option will install/replace the currently installed plugin with [the latest version of Inkay (Roséverse)](https://github.com/Project-Rose/Inkay-Roseverse/releases).
- The "Help/More Info" option will open the Internet Browser to [this support page](https://projectrose.cafe/roseverse_installer/help) to explain and guide you through the process.

In addition, the color of the borders around the app indicates whether you currently have Inkay (Pretendo version), Inkay (Roséverse), or neither installed.
- Purple borders: You have Inkay (Pretendo version) installed.
- Green borders: You have Inkay (Roséverse) installed.
- Grey borders: You have neither a plugin installed nor could we detect the plugin. In other words, you're connected to the Nintendo Network.

## Installing/Updating Plugins
1. Select either "Juxtaposition" for Inkay (Pretendo version) or "Roséverse" for Inkay (Roséverse).
2. If you have more than 1 Wii U environment installed, select the environment where you want the selected plugin downloaded. Use the D-Pad to navigate and press A to select the environment of choice. If you don't see this screen, it means you only have 1 environment (the default Aroma one), and the download will continue.
> [!NOTE]
> Downloading Inkay (Pretendo version) too many times in a row might cause a rate limit from GitHub; however, feel free to download Inkay (Roséverse) as many times as needed, since it is downloaded from our independent servers. 
3. Once the plugin is downloaded, press B to restart the console so the changes apply.

An Aroma notification should pop up stating "**Using Pretendo Network (Roséverse)**" once Inkay (Roséverse) is successfully installed. Then you can click the Miiverse icon to access Roséverse. Likewise, if it doesn't have "**(Roséverse)**" in the notification, then Inkay (Pretendo version) is installed.

## Inkay (Roséverse) Configuration
To configure Inkay (Roséverse), hold L + Down + Start to open the Wii U Plugin Menu, then scroll down and click Inkay (Roséverse). From here, there are several things that you can configure: 
- Network selection > **Connect to Pretendo Network** - Connect to Pretendo Network and Roséverse instead of the Nintendo Network.
- Other settings > **Reset WaraWara Plaza (for Roséverse)** - Force refresh the WaraWara Plaza and pull new WaraWara Plaza data from Project Rosé servers. This is useful if the Juxtaposition plaza still appears after the first installation.

An Aroma notification should tell you if the patch is enabled every time you boot up your console.

# A note on switching between Juxtaposition and Roséverse
There are no risks when switching between the two services, but since Roséverse is a service different from Juxt, your friend list is separate from the native Wii U one, and your followings/followers are not transferred over; however, this should be an opportunity to meet some new people and some familiar faces!

Something that should be known is that some games save data related or tied in some way to a Miiverse post, for example, in [GAME & WARIO](https://en.wikipedia.org/wiki/Game_%26_Wario), recommended words on Miiverse Sketch are saved in the games save data, so if you made a few words already, and transfer to Roséverse or viceversa, those words will still be there in your save data, limiting the amount of words that can be sent.

User Communities that were favorited or "joined"/created in Juxt will not exist in Roséverse and viceversa, it is recommended to create or join a new tournament ([Mario Kart 8](https://en.wikipedia.org/wiki/Mario_Kart_8)) or gym ([Wii Fit U](https://en.wikipedia.org/wiki/Wii_Fit_U)) in this case. This only affects Wii Fit U and Mario Kart 8. All other games that make use of Miiverse functionality should be safe!

# Credits
- All the people who helped worked on this project, you can see them at https://projectrose.cafe; also, any others who are not mentioned.

# Issues?
> [!NOTE]
If you find any issues with **the installer itself** or have any suggestions, please open a GitHub Issue [here](https://github.com/Project-Rose/Roseverse-Installer/issues). If you find any issues with **Inkay (Roséverse)** or have any suggestions, please open a GitHub Issue [here](https://github.com/Project-Rose/Inkay-Roseverse/issues). If you need support for Roséverse itself or have any questions that aren't answered here, please ask in our [Discord server](https://discord.gg/AaTsXndGu), which is more active.
