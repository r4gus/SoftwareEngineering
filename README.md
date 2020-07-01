# Software Engineering Poject

<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/r4gus/SoftwareEngineering">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Projektor</h3>

  <p align="center">
    A project management software for the Aalen university
    <br />
    <a href="https://github.com/r4gus/SoftwareEngineering"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/r4gus/SoftwareEngineering/issues">Report Bug</a>
    ·
    <a href="https://github.com/r4gus/SoftwareEngineering/issues">Request Feature</a>
  </p>
</p>



<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
  * [Built With](#built-with)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
* [Usage](#usage)
* [Roadmap](#roadmap)
* [Contributing](#contributing)
* [License](#license)
* [Contact](#contact)
* [Acknowledgements](#acknowledgements)



<!-- ABOUT THE PROJECT -->
## About The Project

[![Product Name Screen Shot][product-screenshot]](https://example.com) <!-- screen shot of our program -->

This appliction was created as a practical exercise for our Software Engineering lecture.


### Built With
* [QT](https://www.qt.io) <!-- gui -->
* [Boost](https://www.boost.org) <!-- tests -->

<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running follow these simple example steps.

### Prerequisites
[Download boost](https://www.boost.org/users/download/) from their website and unpack it.
```Bash
unzip boost_1_73_0.zip
```
Then place it at a prefered location. This is required if you want to run the existing tests.

Also [download QT](https://www.qt.io/download-open-source).

### Installation

1. Clone the repo
```sh
git clone https://github.com/r4gus/SoftwareEngineering.git
```
2. Open the project with Qt Creator
3. Within SoftwareEngineeringProject.pro file add the path to the boost framework
```sh
INCLUDEPATH += full/path/to/boost_1_73_0
```
4. See if everything works by running all tests under: __Tools -> Tests -> Run All Tests__


<!-- USAGE EXAMPLES -->
## Usage

The project uses a SQLite database. One can specify the name (and path) of the database
within a config file. The path of the config file can be specified with the
__SE_CONFIG__ environment variable.
```Bash
export SE_CONFIG=/path/to/config/file
```

#### Config file (Windows)
```
C:\path\to\database.sqlite
```

If no config file is specified, the default path
(Windows: C:\Users\\%USERNAME%\Documents\db.sqlite) is used.

<!-- ROADMAP -->
## Roadmap

See the [open issues](https://github.com/r4gus/SoftwareEngineering/issues) for a list of proposed features (and known issues).



<!-- CONTRIBUTING
## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request
-->


LICENSE
## License

Distributed under the MIT License. See `LICENSE` for more information.


<!-- CONTACT -->
## Contact

Project Link: [https://github.com/r4gus/SoftwareEngineering](https://github.com/r4gus/SoftwareEngineering)



<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements
* [GitHub Emoji Cheat Sheet](https://www.webpagefx.com/tools/emoji-cheat-sheet)
* [Img Shields](https://shields.io)
* [Choose an Open Source License](https://choosealicense.com)
* [GitHub Pages](https://pages.github.com)
* [Animate.css](https://daneden.github.io/animate.css)
* [Loaders.css](https://connoratherton.com/loaders)
* [Slick Carousel](https://kenwheeler.github.io/slick)
* [Smooth Scroll](https://github.com/cferdinandi/smooth-scroll)
* [Sticky Kit](http://leafo.net/sticky-kit)
* [JVectorMap](http://jvectormap.com)
* [Font Awesome](https://fontawesome.com)





<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[contributors-shield]: https://img.shields.io/github/contributors/r4gus/SoftwareEngineering?style=flat-square
[contributors-url]: https://github.com/r4gus/SoftwareEngineering/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/r4gus/SoftwareEngineering?style=flat-square
[forks-url]: https://github.com/r4gus/SoftwareEngineering/network
[stars-shield]: https://img.shields.io/github/stars/r4gus/SoftwareEngineering?style=flat-square
[stars-url]: https://github.com/r4gus/SoftwareEngineering/stargazers
[issues-shield]: https://img.shields.io/github/issues/r4gus/SoftwareEngineering?style=flat-square
[issues-url]: https://github.com/r4gus/SoftwareEngineering/issues
[license-shield]: https://img.shields.io/github/license/r4gus/SoftwareEngineering?style=flat-square
[license-url]: https://github.com/r4gus/SoftwareEngineering/blob/master/LICENSE.txt
[product-screenshot]: images/screenshot.png
