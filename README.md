# Iso8583lib [![Build Status](https://travis-ci.org/ecampostrini/iso8583lib.svg?branch=travis-config)](https://travis-ci.org/ecampostrini/iso8583lib)
C++ library to work with Iso8583 messages. Since I didn't find many "generic" solutions in C++ I decided 
to give it a shot and try to implement one.

This is a work in progress.

## Basic concepts
This library aims at providing the basic skeletons and functionality over
which different adaptations of the Iso8583 messages can be implemented, leaving
thus the developer only with the task of filling in the details that are
specific to the particular messages she's working with.

There are 3 main abstractions to work with: the fields, their format and the
message. In the next subsections their implementation details are explained.

### Field formats
The field format determines what characters are allowed in a particular field,
what padding in (left or right) should they have and what character should be
used for the padding.
There is a convenience macro in `include/IsoType` to facilitate the creation
of field format specifiers.

### Data fields
An Iso8583 message can contain up to 128 data fields or data elements.
Companies usually customize the data elements to according to their own
needs so there's no standard data element layout. `Iso8583lib` uses a small class 
hierarchy to cover (many of) the different cases that can be found out there. 
There is an abstract base class called `DataElementBase` which defines an
interface common to all the data fields. There are three subclasses that aim
at dealing with all possible cases (in case this is not true, they can always
be extended by adding another subclass):

  * `DataElement`: it's a class template that represents a simple data element,
    e.g. a seller's name. The valid values as well as the format it has are
    determined by the IsoType that is used as a template argument.
  * `DataElementComposite`: a common customization of the data fields is to
    divide them into simple data elements to store custom information. The
    composite data element as its name indicates allows to do this.
  * `DataElementDecorator`: another common customization is to arbitrarily
    prefix the value from a data field with some special identifier as a way 
    of extending the functionality of the given data element. The abstract
    data element decorator class wraps a DataElementBase and allows to apply
    any kind of decoration in it.

### Message
The Iso8583 message consists of a 4 digit message identifier, 1 or 2 bitmaps
and a set of data fields. The class template `IsoMessage` coordinates all that.
The template parameter has to be a Data Element Factory which will typically
allow to create data elements by means of their id. This makes the message
class flexible since it only has to coordinate the reading or writing of the
message and leave the details of the format of the data elements to the
factory.

## Examples
Please, refer to the `examples` folder to see a toy example.

## Requirements
The only requirement is `C++14`.

## Author
Esteban Campostrini
