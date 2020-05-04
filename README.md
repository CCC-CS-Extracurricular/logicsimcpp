Logic Simulator
====
A class project for CSCS 1320: C/C++ Programming.

View original specification on [this Google Doc](https://docs.google.com/document/d/1PkcU8GcqTFi-gDadkfMgs_z-rDFrKQIjeiOCejwFqBI/edit?usp=sharing). The specification was adapted to Markdown for the purpose of context of the repository.

BinSim Project Specification
===

# 1. Introduction

## 1.1. Purpose

### 1.1.1.
BinSim is a project for students learning to develop software. This document demonstrates the concept of a Software Requirements Specification (SRS) and is intended to 1) guide students through this project, and 2) serve as an example for students creating their own SRS in future projects. Ideally, both implementation of this project and design of future projects will be performed in a collaborative environment. No credit will be granted to anyone if only one, or two, people contribute to the project.

### 1.1.2.
BinSim reads a netlist file to create an interconnected structure which may be evaluated with binary logic.

## 1.2. Scope

### 1.2.1.
It is expected that students have a solid understanding of binary logic.

## 1.3. Definitions, acronyms, and abbreviation

### 1.3.1.
Netlist: A document consisting of a list of the electronic components in a circuit and a list of the nodes they are connected to.

## 1.4. References

### 1.4.1.
IEEE 830:1998 Recommended Practice for Software Requirements Specifications

### 1.4.2.
RFC2119 Key words for use in RFCs to Indicate Requirement Levels

# 2. Specific Requirements 

## 2.1. Functions

### 2.1.1.
Requirement level keywords should be interpreted according to RFC2119.

### 2.1.2.
All logical operations shall derive from preferred and other symbols. 

### 2.1.3.
Default preferred and other symbols shall be assigned at compile time, and should be changeable at run time.

### 2.1.4.
Not, And, and Or shall be only native logical elements provided.

### 2.1.5.
Not, And, and Or shall be independent of implementation language built-in logical operations.

### 2.1.6.
Not, And, and Or shall be implemented with comparison operations and selection statements to compare values to preferred and other.

### 2.1.7.
All inputs must be connected to an output for valid circuit evaluation.

### 2.1.8.
Evaluation should occur for individual input changes.

### 2.1.9.
Switch native elements shall have no inputs, only output(s) and a state.

### 2.1.10.
Lamp native elements shall have no outputs, only input(s) and a state.

### 2.1.11.
A count should be kept of native logical elements in a circuit.

### 2.1.12.
A count should be kept of native logical element evaluations.

### 2.1.13.
An in-memory circuit model shall be constructed from reading a netlist file according to the defined Netlist Grammar:

_netlist_:<br>
&emsp;_netlistLine_<br>
&emsp;_netlist_ _netlistLine_

_netlistLine_:<br>
&emsp;_comment_<br>
&emsp;_declaration_<br>
&emsp;_definition_

_comment_:<br>
&emsp;“**//**” `any characters` _newline_

_declaration_:<br>
&emsp;_elementType_ _elementName_ _newline_

_definition_:<br>
&emsp;_elementName_ _signalName_ _elementName_ _signalName_ _newline_

_elementType_:<br>
&emsp;_nativeType_<br>
&emsp;_name_

_elementName_:<br>
&emsp;_name_

_signalName_:<br>
&emsp;_name_

_nativeType_:<br>
&emsp;**not**<br>
&emsp;**and**<br>
&emsp;**or**

_name_:<br>
&emsp;`any of characters a-z or A-Z`<br>
&emsp;“**-**” `hyphen or dash`<br>
&emsp;“**_**” `underscore`

_newline_:<br>
&emsp;`CR`<br>
&emsp;`CRLF`

## 2.2.
Comments shall be ignored from further processing.

## 2.3.
Declarations should specify the existence of an element in a circuit with a unique name.

## 2.4.
Definitions shall specify a connection from a specific element output to a specific element input.

