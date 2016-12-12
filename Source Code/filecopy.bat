@echo on
 
set "sourcePath=%~f1"
set "targetPath=%~f2"
 
echo "SourcePath : %sourcePath%"
echo "TargetPath : %targetPath%"
 
p4 edit "%targetPath%"
p4 add "%targetPath%"
 
@copy "%sourcePath%" "%targetPath%"