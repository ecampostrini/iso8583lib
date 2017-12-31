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
message. The gist of each of them is explained in the following subsections.

### Field formats
The field format determines what characters are allowed in a particular field,
what padding (left or right) should they have and what character should be
used for the padding.
There is a convenience macro in `include/IsoType.hpp` to facilitate the creation
of field format specifiers.

### Data fields
An Iso8583 message can contain up to 128 data fields or data elements.
The standard is not always respected and is not unusual to find tailored
implementations of the standard in which a data element was customized to fit a
particular need.
`Iso8583lib` tries to cover (many of) the different cases that can be found
out there by means of the small class hierarchy defined in `include/DataElement.hpp`.
There is an abstract base class called `DataElementBase` which defines an
interface common to all the data fields. From there, 3 subclasses deal with
possibly the most common scenarios (they can always be extended if needed):

  * `DataElement`: it's a class template that represents a simple data element,
    e.g. a seller's name. The allowed values as well as the format it has are
    determined by the IsoType that is used as a template argument.
  * `DataElementComposite`: a common customization is to divide a particular
    data field into sub-fields to store data in an aggregated fashion.
    The composite data element as its name indicates basically does this.
  * `DataElementDecorator`: another common customization is to arbitrarily
    prefix the value from a data field with some special identifier as a way
    of extending the functionality of the given data element. The abstract
    data element decorator class wraps a DataElementBase and allows to apply
    any kind of decoration to it.

### Message
The Iso8583 message consists of a 4 digit message identifier, 1 or 2 bitmaps
and a set of data fields. The class template from `include/IsoMessage.hpp`
coordinates all that.
The template parameter has to be a Data Element Factory which will typically
allow to create data elements by means of their id. This makes the message
class flexible since it only has to coordinate the reading or writing of the
message and leave the details of the format of the data elements to the
factory.

## Examples
Please, refer to the `examples` folder to see a toy example.

## Requirements
The only requirement is `C++14`.

## TODO's
There are at least some features missing:
  * Currently there's no enforcement over the DataElements that are part of
    a particular message. That could be achieved with a message factory.
  * Similarly, the validation done on the message type identifier is quite lax right
    now. A message factory would also help with this.
  * There's no character encoding handling.

## Author
Esteban Campostrini
