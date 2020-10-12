# **ASUSAR DCM Release V1.0.0.**
___

### Table of Contents 



| index | Topic |
|----|-----|
| 1 |  [Introduction](#1-introduction)|
| 2 |  [Contributors](#2-contributors) |
| 3 |  [Specifications](#3-specifications)|
| 4 |  [Supported functionalities](#4-supported-functionalities)|
| 5 |   [Documentation](#5-documentation)|
| 6 |  [What is next ?](#6-what-is-next) |
|7|  [Copyrights](#7-copyrights) |



___

### 1. Introduction 

This is a bachelor's graduation project that aims to implement the AUTOSAR software modules concerned with the diagnostic services in the Automotive software industry. the main focus is on DCM which is a Basic AUTOSAR module that provides a common API for diagnostic services and the functionality of which is used by external diagnostic tools during the development, manufacturing or service.

The project is a part of **ASUSAR** Project, ASUSAR is a Graduation Project under the Supervision of Prof. Dr. Sherif Hammad and Eng. Muhammad Abdullhay. In which seeking for Developing Open Source Layered Architecture Software Stacks according to AUTOSAR Standard


The Dcm module contains three submodules, Dsd (Diagnostic Service Dispatcher), Dsl (Diagnostic Session Layer) and Dsp (Diagnostic service processing).

#### What is DCM ?
The DCM is a Service Module that responsible for communication between the tester "Client" and the Server "Electronic Control Unit (ECU)" in which the tester request the DCM to process a set of Services to perform Diagnostic Session. This Service have to be processed by three Sub-Modules of DCM, they are DSL, DSD and DSP.


#### DSL

The DSL submodule provides the following functionalities:
* Session handling (as required by ISO14229-1 [2] and ISO 15765-3 [4])
* Application layer timing handling (as required by ISO14229-1 [2] and ISO 15765-
3 [4])
*  Specific response behavior (as required by ISO14229-1 [2] and ISO 15765-3 [4])

  

#### DSD

The DSD submodule is responsible to check the validity of an incoming diagnostic request (Verification of Diagnostic Session/Security Access levels/Application permission) and keeps track of the progress of a service request execution.


#### DSP
the DSP always carries out following basic
process steps:
* analyze the received request message,
* check format and whether the addressed subfunction is supported,
* acquire data or execute the required function call on the DEM, SW-Cs or other
BSW modules
* assemble the response
___

### 2. Contributors

Diagnosis team under the supervision of:
* Eng. Nawaf.
(Email: Nawaf199465@gmail.com)
* Eng. Muhammad Abdullhay. 
(Email: mhmd.a.elhay@gmail.com)
* Prof. Dr. Sherif Hammad.  
(Email: Sherif.hammad@eng.asu.edu.eg) 

Team members are four senior Mechatronics Students :
* Aamer Muhammad Elbaz.
  (Email: aamerelbaz.asurt@gmail.com)
* Ahmed Magdy Ibrahim.
  (Email: Ahmed.magdy19960@gmail.com)
* Ahmed Mahmoud El-Khateeb. 
 (Email: ahmedelkhateeb.asurt@gmail.com)
* Adel Heder Tokan.
  (Email: adeltokan1997@gmail.com)
 
___

### 3. Specifications

Software was developed following certain standards and specifications.

APIs, interfaces, confgurations, code and files structure follows the AUTOSAR **Diagnostic_communication_Manager_SWS** as per version 4.3.1 .

The SWS itself and the diagnostic services are based on the Iso standards **ISO-14229** (Road vehicles- Unified diagnostic
services (UDS) - Specification and
requirements) and **ISO-15765** (Road vehicles - Diagnostics on
Controller Area Networks (CAN) ).

The code was developed using best practice techniques following and abiding by the MISRA rules as per **MISRA C:2012** standard. 




___

### 4. Supported functionalities

As previously mentioned the project focuses on implementing the most basic diagnostic module; the **DCM** ( diagnostic communication manager), and a basic functionality of the **DET** (diagnostic error tracker) module.

However, naturally, not all the services are supported, so this section covers the supported/implemented Services/functionalities.


#### Diagnostic Session Layer (DSL)

The Implemented DSL Sub-Module Supports the Following Features:
1.	##### Request Handling 
    - Forward requests from the PduR module to the DSD submodule.
    - Process Concurrent Tester Present (keep alive logic). 

2. #####	Response Handling 
   * Forward responses from the DSD submodule to the PduR module.
   * Guarantee response timing to tester. 
   * Support of Response Pending response triggered by the application. 

3. #####	Security Level Handling 
   * Manage security level.

4. #####	 Session State Handling 
   * Manage session state. 
   * Keep track of active non-default sessions. 

5. #####	Diagnostic Protocol Handling.
   * Handling of different diagnostic protocols.

 #### Diagnostic Service Dispatcher (DSD)
The Implemented DSD Sub-Module Supports the Following Features:



1.	##### Request Handling 
     * Forward requests from the DSL submodule to the DSP submodule.
     * Delegates Processing of Internal and External requested Services to DSP.
     * Keeps track of the Progress of a Service Request Execution.

2.	##### Response Handling 
     * Transmits the response of the Application -received by DSP- to the DSL submodule.
     * Handles Positive and Negative Responses resulted of Service and forward it to DSL submodule.

3.	##### Request Service Validation Handling
     * Handles Validation of requested Service Identifier.
     * Validate the Session and Security Permissions of requested Services.

#### Diagnostic Service Processing (DSP)
The Implemented DSP Sub-Module Supports the Following Features:

1. #####	Request Handling 
    * Receives the Service from the DSD submodule and Delegates Application Interfaces.
    * Verifies Service Formatting and Checks Sub-Service Identifier.
    * Managing of Synchronous and Asynchronous Call Behaviors.
    * Handles Different Statuses of RTE use port to Interface the Application such as (Synchronous/Asynchronous Client-Server, Synchronous/Asynchronous Function…etc.).

2. #####	Response Handling 
    * Handles Negative Response Codes Generated from Services.
    
#### UDS Supported Services:

| Supported Service |
|----------|
|DiagnosticSessionControl (10 hex) service|
|TesterPresent (3E hex) service|
|ReadDataByIdentifier (22 hex) service
|WriteDataByIdentifier (2E hex) service
|SecurityAccess (27 hex) service |
|RoutineControl (31 hex) service

1. #####	Diagnostic Session Control		Service ID: 0x10
Service 0x10 allows an external tester to enable different diagnostic sessions in the server. A diagnostic session enables a specific set of diagnostic services and/or functionality in the server.

2. #####	Tester Present				Service ID: 0x3E
   
This service is used to keep one or multiple servers in a diagnostic session being different than the default Session.

3. #####	Read Data by Identifier			Service ID: 0x22

This service allows the client to request data record values from the server identified by one or more data Identifiers.

4. #####	Write Data by Identifier			Service ID: 0x2E
   
service allows the client to write information into the server at an internal location specified by the provided data identifier.

5. #####	Security Access				Service ID: 0x27
   
this service is to provide a means to access data and/or diagnostic services which have restricted access for security.

6. #####	Routine Control				Service ID: 0x31
   
A service to start, stop or obtain the results of a routine execution identified by a specific routine Identifier from Application.







___




### 5. Documentation

To facilitate understanding of the code and to encourage future contributions to the project, we did not just rely on in-code comments due to the hard time it gives to whoever want to explore a software project in general. Hence the code was commented on and documented using **Doxygen** tool. Here is a link to the generated documentation 

___
### 6. What is Next

This section covers our recommendations for future contributors to the project and what is the next step to take.

___
### 7. Copyrights

>Copyright (c) 2019-2020 Faculty of Engineering Ain Shams University 