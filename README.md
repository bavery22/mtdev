mtdev
=====
Master branch has only this Readme.md
Contains Android makefiles for klaatu in the klaatu branch. 
The upstream points to bitmath so we can stay in sync. 
To keep in sync do:
  git fetch upstream
  git merge upstream/klaatu
The Klaatu branch can be dropped into AOSP in external/mtdev and 
    built using mm.
