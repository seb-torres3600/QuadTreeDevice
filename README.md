# QuadTreeDevice
### Author: Sebastian Torres

### Dockerized for convenience

- This code should run on a device held by a user. It's original intent was that it could be loaded on an ankle monitor to see if an individual is staying in the bounds they must stay in. 

- This is a quad tree implementation. We receive boundaries and build the tree from reaching out to the QuadTreeAPI

- There is a config file that allows us to the define the size of the area we which to have around our user and when to refresh the quad tree depenending on how close to the boundary edge the user gets. 

- Logs are being sent to logs/logs.log, this can be changed by passing an option to the commandline, the log level can also be configured by the command line 



