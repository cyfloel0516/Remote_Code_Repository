# Remote_Code_Repository

# Introduction
This project is for the final project of my SU_CIS687 Object Oriented Design course. 
It is a application in C/S architecture which allows user to manage the C++ package to a repository server. It uses the code dependency analysis function in another project(<https://github.com/cyfloel0516/Dependency-Analyser>) to archieve the automatic dependency management.

# Client
1. User uses client application to upload C++ package(a folder)
2. User can check the dependency of the package
3. User can download package from server with or withour the dependency packages
4. User can close the package to prevent further modification. Once a package is closed, new modification will create a new version

# Server
1. Save package from user in server
2. Use multiple threads to do automatic dependency analysis when a new package check in
3. Do version control by the package status(opened or closed) and upload date

# Other detail
Please check readme.pdf file

Related Project(Code Analyze): <https://github.com/cyfloel0516/Dependency-Analyser>
