# bsng-plugins
[CodeWarrior 6](https://macintoshgarden.org/apps/codewarrior-6) project files of various [BSNG](https://macintoshgarden.org/apps/bucks-serial-number-generator-bsng) plugins I made for [macintoshgarden.org](https://macintoshgarden.org/)

Each file is compressed with [StuffIt 5.5](https://macintoshgarden.org/apps/stuffit-expander-55) to preserve resource forks. Each archive contains the project files for that plugin. [Buck's Serial Number Generator (BSNG) 2.6+](https://macintoshgarden.org/apps/bucks-serial-number-generator-bsng) is required to actually use the plugins. They will also only work with Classic versions of BSNG.

# Important Files
* **[/Libraries](Libraries/)**: Contains various libraries necessary to compile the plugins.
  * Note that **[/Libraries/standard utils](/Libraries/standard%20utils)** is updated regularly.
* **/_My * Template.sit**: Contains the template files I use to quickly make plugins.

# Creating Your Own Plugins
The quickest way to get started is to choose a template from **/_My * Template.sit**.  You will also need everything under **[/Libraries](Libraries/)**. Also, download [BSNG DeRez](https://github.com/siddhartha77/bsng-derez).

Once you are ready to go, drag and drop the application you are writing a plugin for onto BSNG DeRez to generate **BSNG Product.h** and **BSNG Product.r**. Add these two files to your project and verify that **BSNG Product.h** contains the correct information for the application. **BSNG Product.r** will contain the application's icon.

With your project now setup, writing the actual code is done in **Template.c** in the **DoCalc()** routine.
