
         Oracle Media Exchange Product Overview Documentation.

  This is one of the documentation files mandated by the Porting Kit
 Guidelines specification.  It contains the most general information
                          about the product.

1. Product Name and Version Number
 
     The name of this product is Oracle Media Exchange.  This document
     pretains to version 3.0 of Oracle Media Exchange and above.  See the 
     file $SRCHOME/mx/doc/release.doc for the exact version number.  The 
     standard abbreviation for Oracle Media Exchange is MX, and is used as 
     such throughout this document.

2. Overview

     See the hardcopy documentation "Orace Media Server Porting Kits Overview"
     for a description of how the various OMS products are related. The 
     document is available from [ some AA ] in [ some cube ]. 

     The document you are now reading describes only the MX product.

     The MX product provides the communications substrate between the
     distributed processes of the Oracle Media Server.  It provides both a
     remote procedure call (RCP) inteface, and a CORBA-compliant object 
     oriented interface.

3. Dependencies

     See the file $SRCHOME/mx/doc/porting.doc for a list of OS requirements.

     See the file $SRCHOME/mx/doc/release.doc of a list of Oracle Dependencies.

4. Architecture

     Within the MX product, there are the following components:

        YS   - Services Layer.
        MT   - Media Tools layer.
        MN   - Media Net component, RPC interface.
        MISC - Other services, mostly logging
        YC   - Object-oriented IDL compiler
        YD   - Object-oriented routing daemon
        YE   - Object-oriented event channels
        YO   - Object-oriented object layer
        YR   - Object-oriented interface repository
        YT   - Object-oriented transport interface to MN layer.

     The components are layered as follows:

        +--------+------+------+------+------+------+-----+
        |        | YC   |  YD  |  YE  |  YO  |  YR  | YT  |  
        | MISC   +----------------------------------------+
        | Layer  |     YT Layer                           |      
        +--------+----------------------------------------+
        |     MN Layer                                    |
        +-------------------------------------------------+
        |         MT Layer                                |
        +-------------------------------------------------+
        |         YS Layer                                |
        +-------------------------------------------------+

4.1  YS Component

     The YS component provides the general interface to the operating system.
     It is in many ways, "OMS CORE".  It provides the interface to the
     basic functions of the operating system.

4.2  MT Component

     The MT component adds additional useful interfaces for logging, error
     handling, and the like.  MT is a transitional interface; all of its
     functionality will be incorporated into the YS layer in a future
     release, and MT will become obsolete and will be removed.  MT currently 
     duplicates some functionality also found in YS.  This is to allow the 
     conversion of existing generic code to the new YS interfaces to take
     place over an extended period of time.

4.3  MN Component

     The MN component provides the connectivity between processes running
     on the various systems in an OMS deployment.  It provides the RPC based
     interface.  

4.4  MISC Component

     The MISC component provides common RPC-based servers, such as the
     distributed logger, as well as some common modules for statistic
     gathering which need to link into all OMS processess.

4.5  YC, YD, YE, YO, YR, and YT Components

     These components provide the distributed Object-oriented interface.  This
     inteface is sometimes known as the "ORB-based" interface.
     
5. Directory Structure and Files

     See the $SRCHOME/mx/doc/porting.doc document for details.

6. OSD files.

     See the $SRCHOME/mx/doc/porting.doc document for details.

7. Porting Process

     See the $SRCHOME/mx/doc/porting.doc document for details.

8. Testing Process

     See the $SRCHOME/mx/doc/porting.doc document for details.

9. Files shipped

     [ Input from integration group required here ]

10. Installation instructions

     Installation of MX is acomplished via the use of the standard orainst
     portable installer.
  

