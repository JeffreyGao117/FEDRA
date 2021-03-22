# Framework for Emulsion Data Reconstruction(FEDRA)

This repo contains the FEDRA framework and files for virtual recreation(virtualrec) of muon neutrino particle tracks.

The storage, calibration, reconstruction, analysis and visualization of massive lead/emulsion target for a long-baseline neutrino oscillation search data is the task of FEDRA system written in C++ and based on ROOT framework.

FEDRA documentation is located in /Fedra/htmldoc

[[_TOC_]]

## Instructions for setup
Some steps require you to replace <path_to_something> with your system specific absolute path.

### Windows 10 Home/Pro
___
1. Install [cmake](https://cmake.org/download/)
1. Download [ROOT 5](https://root.cern.ch/content/release-53438) (use Windows Visual Studio 2013 .exe link)
2. Open a shell and run `source <path-to-root>/root_v5.34.38/bin/thisroot.sh`
3. Install [Visual Studio Community 2013](https://visualstudio.microsoft.com/vs/older-downloads/)
4. Copy win32.mak from [this Link](https://gist.github.com/ynkdir/688e62f419e5374347bf)
    - Place win32.mak into `C:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/include`
5. Open "Edit the system environment variables" in control panel
     - Click "Environment variables"-->"New" to create the variables listed below
        | Variable Name | Variable Value |
        |----|----|
        | rootsys | &lt;path-to-root&gt;\root_v5.34.38 |
        | fedra_root | &lt;path-to-Fedra&gt;\Fedra |
        | include | %fedra_root%\include;%rootsys%\include;%include% |
        | lib | %rootsys%\lib;%fedra_root%\lib;%lib% |
     - Then click on the already existing variable "Path"-->"Edit"-->"New"
     - Add `%fedra_root%\bin;%fedra_root%\lib;%rootsys%\bin;%path%`
     - Apply changes and close
6. Open x86 command prompt from VS2013 
    - Located here: `C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts\Developer Command Prompt for VS2013.lnk`
7. `cd <path-to-Fedra>\Fedra/src/libVt++`
8. `nmake -f Makefile.w32 clean`
9. `nmake -f Makefile.w32`
10. `cd ../..`
11. `regsvr32 SySalDataIO.dll`
11. `install.cmd`
    - If encounter error that system is not registered, follow steps below:
        1. Search "Registsry Editor" in Windows start
        1. In top right, "Edit"-->"Find": SySalSataIO
        1. In /Fedra/install.cmd on line 110, change path accordingly
            - E.g. `@%regutil% query HKEY_LOCAL_MACHINE\SOFTWARE\SySal2\Classes\SySalDataIO`
12. `cd src`
13. `makeall`

Running virtualrec:
1. `cd ../../FnuG4`
2. `root FnuG4.C++`
3. `.q`
4. `root virtualrec.c`
5. `display()`


### Linux - Ubuntu/CentOS
___
Currently does not work. ROOT 5.34.38 is not available on LXPlus so ROOT 5.34.36 is used instead. Utilizes Docker(https://www.docker.com/):
1. `docker pull centos:7`
1. `docker run -it --name virtualrec centos:7 /bin/bash`
1. `yum install git cmake make gcc-c++ gcc binutils libX11-devel libXpm-devel libXft-devel libXext-devel openssl-devel`
    - optional text editor `vim`
1. `yum -y update`
1. `git clone --branch v5-34-00-patches https://github.com/root-project/root.git root_src`
1. `mkdir root_build root_install && cd root_build`
1. `cmake -DCMAKE_INSTALL_PREFIX=../root_install ../root_src`
1. `cmake --build . -- install`
1. `source ../root_install/bin/thisroot.sh`
1. `cd ..`
1. `git clone https://gitlab.cern.ch/jegao/virtualrec.git`
1. `cd virtualrec/Fedra`
1. `./install.sh`
    - if permission denied in previous step, `chmod +x install.sh`, then `./install.sh`
1. `source ./setup_new.sh`
1. `cd ../FnuG4`
1. `root.exe FnuG4.C++`
1. `.q`
1. `root.exe virtualrec.C`
1. Command not implemented, Docker no graphics: `display()`


### LXPlus
___
Currently does not work:
1. `source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/5.34.36/x86_64-centos7-gcc48-opt/root/bin/thisroot.sh`
2. `git clone https://gitlab.cern.ch/jegao/virtualrec.git`
3. `cd virtualrec/Fedra/`
4. `../install.sh`
5. `cd ../FnuG4/`
6. `root -b -q FnuG4.C`
7. `root virtualrec.c`
8. `display()`

<br>

## Appendix
### Windows Environment:
- Windows 10 Home v.2004(19041.572)
- Microsoft Visual Studio 2013
- ROOT 5.35.38
- C++98
- cmake version 3.18.0-rc2

### Linux Environment:
- CentOS 7
- ROOT 5.34.36
- C++11
- gcc version 4.8.5 20150623 (Red Hat 4.8.5-39)
- cmake version 2.8.12.2
