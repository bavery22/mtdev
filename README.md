mmtdev    <br>
=====    <br>
Master branch has only this Readme.md    <br>
Contains Android makefiles for klaatu in the klaatu branch.    <br>
The upstream points to bitmath so we can stay in sync.    <br>
To keep in sync do:    <br>
  git remote add upstream http://bitmath.org/git/mtdev.git    <br>
  git fetch upstream    <br>
  git checkout -b klaatu --track origin/klaatu    <br>
  git merge upstream/master    <br>
    <br>
The Klaatu branch can be dropped into AOSP in external/mtdev and built using mm.

