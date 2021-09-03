



# proto2教程

本指南描述了如何使用协议缓冲语言来构造协议缓冲数据，包括.proto文件语法以及如何从.proto文件中生成数据访问类。它涵盖了协议缓冲语言的proto2版本:关于proto3语法的信息，请参阅proto3语言指南。

这是一个参考指南 - 有关使用本文档中描述的许多功能的步骤示例，请参阅所选语言的教程。

## 定义消息类型

首先让我们看一下一个非常简单的例子。 假设您希望定义搜索请求消息格式，其中每个搜索请求都有一个查询字符串，您对此感兴趣的结果的特定页面以及每个页面的数量。 以下是您用来定义消息类型的.proto文件。

```protobuf
message SearchRequest {
  required string query = 1;
  optional int32 page_number = 2;
  optional int32 result_per_page = 3;
}
```

SearchRequest消息定义指定三个字段（名称/值），一个用于在此类型的消息中包含的数据。 每个字段都有一个名称和类型。

### 指定字段类型

在上面的示例中，所有字段都是标量类型:两个整数(page_number` and `result_per_page)和一个字符串(query)。但是，您也可以为字段指定复合类型，包括枚举和其他消息类型。

### 分配字段号码

如您所见，消息定义中的每个字段都具有唯一的编号。  这些数字用于识别消息二进制格式的字段，并且在使用邮件类型时，不应更改。  请注意，1到15范围内的字段编号将需要一个字节编码，包括字段编号和字段类型（您可以在协议缓冲区编码中找到有关此内容的更多信息）。   16到2047范围内的字段数字采用两个字节。 因此，您应该为非常经常发生的消息元素保留字段编号1到15。  请记住为未来可能添加的经常发生的元素留下一些空间。

 

您可以指定的最小字段数为1，最大的是2^29  -   1，或536,870,911。 您也无法使用19000至19999（FieldDescriptor ::  KfirstreservedNumber至FieldDescriptor ::  KlastreservedNumber），因为它们被保留为协议缓冲区实现，如果您在.proto中使用这些保留的数字之一，则协议缓冲区编译器将抱怨。 同样，您不能使用任何先前保留的字段编号。

### 指定字段规则

您将消息字段指定为以下字段之一

- `required`: 一个格式良好的消息必须恰好有一个此字段。
- `optional`:一个格式良好的消息可以有0个或一个此字段(但不能超过一个)。
- `repeated`: 该字段可以在形成良好的消息中重复任何次数（包括零）。 重复值的顺序将被保留。

出于历史原因，标准数字类型的重复字段（例如，INT32，INT64，ENUM）没有按照它们的方式进行高效编码。 新代码应使用特殊选项[packed = true]以获得更高效的编码。 例如：

```protobuf
repeated int32 samples = 4 [packed=true];
repeated ProtoEnum results = 5 [packed=true];
```

You can find out more about `packed` encoding in [Protocol Buffer Encoding](https://developers.google.com/protocol-buffers/docs/encoding#packed).

如果在某个时候，您希望停止写入或发送required字段，将该字段更改为optional字段将会有问题，老读者会认为没有此字段的消息是不完整的，可能会无意中拒绝或删除它们。您应该考虑为您的缓冲区编写特定于应用程序的自定义验证例程。

required字段的第二个问题出现在向枚举添加值时。在这种情况下，无法识别的枚举值将被视为缺失，这也会导致required值检查失败。

### 添加更多消息类型

可以在一个.proto文件中定义多个消息类型。这在定义多个相关消息时非常有用，例如，如果希望定义与SearchResponse消息类型对应的回复消息格式，则可以将其添加到相同的.proto中.

```protobuf
message SearchRequest {
  required string query = 1;
  optional int32 page_number = 2;
  optional int32 result_per_page = 3;
}

message SearchResponse {
 ...
}
```

虽然可以在一个.proto文件中定义多个消息类型(如message、enum和service)，但如果在一个文件中定义了大量依赖关系不同的消息，也会导致依赖关系膨胀。建议在.proto文件中包含尽可能少的消息类型。

### 添加评论

要为您的.proto文件添加注释，请使用c / c ++  -  style //和/ * ... * /语法。

```protobuf
/* SearchRequest represents a search query, with pagination options to
 * indicate which results to include in the response. */

message SearchRequest {
  required string query = 1;
  optional int32 page_number = 2;  // Which page number do we want?
  optional int32 result_per_page = 3;  // Number of results to return per page.
}
```

### 保留字段

如果通过完全删除字段更新消息类型，或者注释出来，将来可以在对类型进行自己的更新时重复使用字段编号。 如果稍后加载相同的旧版本，这可能会导致严重问题.proto，包括数据损坏，隐私错误等。  确保不会发生这种情况的一种方法是指定要保留删除字段的字段编号（和/或名称，也可能导致JSON序列化问题）。  协议缓冲区编译器如果未来的用户尝试使用这些字段标识符，将报错。

```protobuf
message Foo {
  reserved 2, 15, 9 to 11;
  reserved "foo", "bar";
}
```

注意，不能在同一个保留语句中混合字段名和字段号。

### What's Generated From Your `.proto`?

When you run the [protocol buffer compiler](https://developers.google.com/protocol-buffers/docs/proto#generating) on a `.proto`, the compiler generates the code in your chosen language you'll need to  work with the message types you've described in the file, including  getting and setting field values, serializing your messages to an output stream, and parsing your messages from an input stream.

- For **C++**, the compiler generates a `.h` and `.cc` file from each `.proto`, with a class for each message type described in your file.
- For **Java**, the compiler generates a `.java` file with a class for each message type, as well as special `Builder` classes for creating message class instances.
- **Python** is a little different – the Python compiler generates a module with a static descriptor of each message type in your `.proto`, which is then used with a *metaclass* to create the necessary Python data access class at runtime.
- For **Go**, the compiler generates a `.pb.go` file with a type for each message type in your file.

You can find out more about using the APIs for each language by  following the tutorial for your chosen language. For even more API  details, see the relevant [API reference](https://developers.google.com/protocol-buffers/docs/reference/overview).

## 标量值类型

标量消息字段可以具有以下类型之一，表中显示了.proto文件中指定的类型，以及自动生成的类中相应的类型

| .proto Type | Notes                                                        | C++ Type | Java Type  | Python Type[2]                       | Go Type  |
| ----------- | ------------------------------------------------------------ | -------- | ---------- | ------------------------------------ | -------- |
| double      |                                                              | double   | double     | float                                | *float64 |
| float       |                                                              | float    | float      | float                                | *float32 |
| int32       | Uses variable-length encoding. Inefficient for encoding negative  numbers – if your field is likely to have negative values, use sint32  instead. | int32    | int        | int                                  | *int32   |
| int64       | Uses variable-length encoding. Inefficient for encoding negative  numbers – if your field is likely to have negative values, use sint64  instead. | int64    | long       | int/long[3]                          | *int64   |
| uint32      | Uses variable-length encoding.                               | uint32   | int[1]     | int/long[3]                          | *uint32  |
| uint64      | Uses variable-length encoding.                               | uint64   | long[1]    | int/long[3]                          | *uint64  |
| sint32      | Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int32s. | int32    | int        | int                                  | *int32   |
| sint64      | Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int64s. | int64    | long       | int/long[3]                          | *int64   |
| fixed32     | Always four bytes. More efficient than uint32 if values are often greater than 228. | uint32   | int[1]     | int/long[3]                          | *uint32  |
| fixed64     | Always eight bytes. More efficient than uint64 if values are often greater than 256. | uint64   | long[1]    | int/long[3]                          | *uint64  |
| sfixed32    | Always four bytes.                                           | int32    | int        | int                                  | *int32   |
| sfixed64    | Always eight bytes.                                          | int64    | long       | int/long[3]                          | *int64   |
| bool        |                                                              | bool     | boolean    | bool                                 | *bool    |
| string      | A string must always contain UTF-8 encoded or 7-bit ASCII text. | string   | String     | unicode (Python 2) or str (Python 3) | *string  |
| bytes       | May contain any arbitrary sequence of bytes.                 | string   | ByteString | bytes                                | []byte   |

You can find out more about how these types are encoded when you serialize your message in [Protocol Buffer Encoding](https://developers.google.com/protocol-buffers/docs/encoding).

[1] In Java, unsigned 32-bit and 64-bit integers are  represented using their signed counterparts, with the top bit simply  being stored in the sign bit.

[2] In all cases, setting values to a field will perform type checking to make sure it is valid.

[3] 64-bit or unsigned 32-bit integers are always  represented as long when decoded, but can be an int if an int is given  when setting the field. In all cases, the value must fit in the type  represented when set. See [2].

## 可选字段和默认值

如上所述，消息描述中的元素可以被标记为可选的。格式良好的消息可能包含也可能不包含可选元素。当一个消息被解析时，如果它不包含一个可选元素，那么被解析对象中相应的字段将被设置为该字段的默认值。可以将默认值指定为消息描述的一部分。例如，假设您希望为SearchRequest的每个页面值的结果提供缺省值10。

```protobuf
optional int32 result_per_page = 3 [default = 10];
```

如果未为可选元素指定默认值，则使用类型的默认值：对于字符串，默认值是空字符串。 对于字节，默认值是空字节字符串。 对于Bool，默认值为false。 对于数字类型，默认值为零。  对于枚举，默认值是枚举类型定义中列出的第一个值。 在将值添加到枚举值列表的开头时，必须小心。  有关如何安全更改定义的指南，请参阅更新消息类型部分。

## 枚举

当您定义消息类型时，您可能希望其中一个字段仅具有预定义的值列表之一。  例如，假设您要为每个搜索查询添加一个语料库字段，其中语料库可以是通用的，网络，图像，本地，新闻，产品或视频。  您可以简单地通过向邮件定义添加枚举来执行此操作 -  具有枚举类型的字段只能具有指定的一组常量作为其值（如果您尝试提供不同的值，则解析器将像素处理 一个未知的字段）。  在以下示例中，我们添加了一个名为corpus的枚举，其中包含所有值，以及类型语料库的字段：

```protobuf
message SearchRequest {
  required string query = 1;
  optional int32 page_number = 2;
  optional int32 result_per_page = 3 [default = 10];
  enum Corpus {
    UNIVERSAL = 0;
    WEB = 1;
    IMAGES = 2;
    LOCAL = 3;
    NEWS = 4;
    PRODUCTS = 5;
    VIDEO = 6;
  }
  optional Corpus corpus = 4 [default = UNIVERSAL];
}
```

您可以通过为不同的枚举常量分配相同的值来定义别名。 为此，您需要将Allow_alias选项设置为True，否则，协议编译器将在找到别名时生成错误消息。

```protobuf
enum EnumAllowingAlias {
  option allow_alias = true;
  UNKNOWN = 0;
  STARTED = 1;
  RUNNING = 1;
}
enum EnumNotAllowingAlias {
  UNKNOWN = 0;
  STARTED = 1;
  // RUNNING = 1;  // Uncommenting this line will cause a compile error inside Google and a warning message outside.
}
```

枚举器常量必须在32位整数的范围内。  由于枚举值在线使用Varint编码，因此负值效率低，因此不建议使用。 您可以在消息定义中定义枚举，如上面的示例或外部 -  这些枚举可以在您的.proto文件中的任何消息定义中重用。  您还可以使用一条消息中声明的枚举类型作为不同消息中的字段的类型，使用语法_MessageType _._ enumtype_。

在使用枚举的.proto上运行协议缓冲区编译器时，生成的代码将为java或c ++具有相应的枚举，或者用于python的特殊enumdescriptor类，用于创建具有整数值的一组符号常量 运行时生成的类。

For more information about how to work with message `enum`s in your applications, see the [generated code guide](https://developers.google.com/protocol-buffers/docs/reference/overview) for your chosen language.

## 保留值

如果通过完全删除枚举输入或注释出来更新枚举类型，则将未来的用户可以在对类型进行自己的更新时重用数值。 如果稍后加载相同的旧版本，这可能会导致严重问题.proto，包括数据损坏，隐私错误等。  确保不会发生这种情况的一种方法是指定要保留删除条目的数值（和/或也可能导致JSON序列化问题的名称）。  协议缓冲区编译器将抱怨如果未来的用户尝试使用这些标识符。 您可以使用MAX关键字指定您的保留数值范围达到最大可能值。

```protobuf
enum Foo {
  reserved 2, 15, 9 to 11, 40 to max;
  reserved "FOO", "BAR";
}
```

请注意，您无法在同一保留语句中混合字段名称和数字值。

## 使用其他消息类型

您可以使用其他消息类型作为字段类型。 例如，假设您想要在每个SearchResponse消息中包含Result消息 - 要执行此操作，可以在同一.proto中定义结果消息类型，然后在searchResponse中指定Result的字段：

```protobuf
message SearchResponse {
  repeated Result result = 1;
}

message Result {
  required string url = 1;
  optional string title = 2;
  repeated string snippets = 3;
}
```

### 导入定义

请注意，Java中不可用此功能。

在上面的示例中，结果消息类型在与searchResponse中的相同文件中定义 - 如果要用作字段类型的消息类型已在另一个.proto文件中定义？ 

您可以通过导入它们来使用其他.proto文件的定义。 要导入另一个.proto的定义，请将导入语句添加到文件顶部：

```protobuf
import "myproject/other_protos.proto";
```

默认情况下，您只能使用直接导入的.proto文件中的定义。但是，有时您可能需要将.proto文件移动到新位置。与直接移动.proto文件并在单个更改中更新所有调用站点不同，现在您可以在旧位置放置一个虚拟的.proto文件，以使用import public概念将所有导入转发到新位置。任何导入包含Import public语句的原型的人都可以传递地依赖于导入公共依赖项。例如

```protobuf
// new.proto
// All definitions are moved here
```

```protobuf
// old.proto
// This is the proto that all clients are importing.
import public "new.proto";
import "other.proto";
```

```protobuf
// client.proto
import "old.proto";
// You use definitions from old.proto and new.proto, but not other.proto
```

协议编译器在协议编译器命令行中指定的一组目录中搜索导入的文件使用-I/  -  proto_path标志。 如果没有给出任何标志，它会在调用编译器的目录中。 通常，您应该将-proto_path标志设置为项目的根目录，并为所有导入使用完全限定的名称。

### Using proto3 Message Types

可以导入proto3消息类型并在PROTO2消息中使用它们，反之亦然。 但是，PROTO2枚举不能用于PROTO3语法。

## 嵌套类型

您可以在其他消息类型中定义和使用消息类型，如下面的示例所示，Result消息是在SearchResponse消息中定义的

```protobuf
message SearchResponse {
  message Result {
    required string url = 1;
    optional string title = 2;
    repeated string snippets = 3;
  }
  repeated Result result = 1;
}
```

如果要在其父消息类型之外重用此消息类型，则将其称为_parent _._ type_：

```protobuf
message SomeOtherMessage {
  optional SearchResponse.Result result = 1;
}
```

您可以根据您的喜好嵌套消息：

```protobuf
message Outer {       // Level 0
  message MiddleAA {  // Level 1
    message Inner {   // Level 2
      required int64 ival = 1;
      optional bool  booly = 2;
    }
  }
  message MiddleBB {  // Level 1
    message Inner {   // Level 2
      required int32 ival = 1;
      optional bool  booly = 2;
    }
  }
}
```

## Groups

请注意，此特性已弃用，不应该在创建新消息类型时使用，而是使用嵌套消息类型。

组是留言定义中信息的另一种方法。 例如，另一种指定包含许多Result的SearchResponse的方法如下：

```protobuf
message SearchResponse {
  repeated group Result = 1 {
    required string url = 2;
    optional string title = 3;
    repeated string snippets = 4;
  }
}
```

A group simply combines a nested message type and a field into a  single declaration. In your code, you can treat this message just as if  it had a `Result` type field called `result` (the latter name is converted to lower-case so that it does not  conflict with the former). Therefore, this example is exactly equivalent to the `SearchResponse` above, except that the message has a different [wire format](https://developers.google.com/protocol-buffers/docs/encoding).

## 更新消息类型

例如，如果现有的消息类型不再满足您的所有需求，您希望消息格式有一个额外的字段，但您仍然希望使用使用旧格式创建的代码，不要担心!在不破坏任何现有代码的情况下更新消息类型是非常简单的。只要记住以下规则：

- Don't change the field numbers for any existing fields.
- Any new fields that you add should be `optional` or `repeated`. This means that any messages serialized by code using your "old"  message format can be parsed by your new generated code, as they won't  be missing any `required` elements. You should set up sensible [default values](https://developers.google.com/protocol-buffers/docs/proto#optional) for these elements so that new code can properly interact with messages generated by old code. Similarly, messages created by your new code can be parsed by your old code: old binaries simply ignore the new field  when parsing. However, the unknown fields are not discarded, and if the  message is later serialized, the unknown fields are serialized along  with it – so if the message is passed on to new code, the new fields are still available.
- Non-required fields can be removed, as long as the field number is  not used again in your updated message type. You may want to rename the  field instead, perhaps adding the prefix "OBSOLETE_", or make the field  number [reserved](https://developers.google.com/protocol-buffers/docs/proto#reserved), so that future users of your `.proto` can't accidentally reuse the number.
- A non-required field can be converted to an [extension](https://developers.google.com/protocol-buffers/docs/proto#extensions) and vice versa, as long as the type and number stay the same.
- `int32`, `uint32`, `int64`, `uint64`, and `bool` are all compatible – this means you can change a field from one of  these types to another without breaking forwards- or  backwards-compatibility. If a number is parsed from the wire which  doesn't fit in the corresponding type, you will get the same effect as  if you had cast the number to that type in C++ (e.g. if a 64-bit number  is read as an int32, it will be truncated to 32 bits).
- `sint32` and `sint64` are compatible with each other but are *not* compatible with the other integer types.
- `string` and `bytes` are compatible as long as the bytes are valid UTF-8.
- Embedded messages are compatible with `bytes` if the bytes contain an encoded version of the message.
- `fixed32` is compatible with `sfixed32`, and `fixed64` with `sfixed64`.
- For `string`, `bytes`, and message fields, `optional` is compatible with `repeated`. Given serialized data of a repeated field as input, clients that expect this field to be `optional` will take the last input value if it's a primitive type field or merge  all input elements if it's a message type field. Note that this is **not** generally safe for numeric types, including bools and enums. Repeated fields of numeric types can be serialized in the [packed](https://developers.google.com/protocol-buffers/docs/encoding#packed) format, which will not be parsed correctly when an `optional` field is expected.
- Changing a default value is generally OK, as long as you remember  that default values are never sent over the wire. Thus, if a program  receives a message in which a particular field isn't set, the program  will see the default value as it was defined in that program's version  of the protocol. It will NOT see the default value that was defined in  the sender's code.
- `enum` is compatible with `int32`, `uint32`, `int64`, and `uint64` in terms of wire format (note that values will be truncated if they  don't fit), but be aware that client code may treat them differently  when the message is deserialized. Notably, unrecognized `enum` values are discarded when the message is deserialized, which makes the field's `has..` accessor return false and its getter return the first value listed in the `enum` definition, or the default value if one is specified. In the case of  repeated enum fields, any unrecognized values are stripped out of the  list. However, an integer field will always preserve its value. Because  of this, you need to be very careful when upgrading an integer to an `enum` in terms of receiving out of bounds enum values on the wire.
- In the current Java and C++ implementations, when unrecognized `enum` values are stripped out, they are stored along with other unknown  fields. Note that this can result in strange behavior if this data is  serialized and then reparsed by a client that recognizes these values.  In the case of optional fields, even if a new value was written after  the original message was deserialized, the old value will be still read  by clients that recognize it. In the case of repeated fields, the old  values will appear after any recognized and newly-added values, which  means that order will not be preserved.
- Changing a single `optional` value into a member of a **new** `oneof` is safe and binary compatible. Moving multiple `optional` fields into a new `oneof` may be safe if you are sure that no code sets more than one at a time. Moving any fields into an existing `oneof` is not safe.
- Changing a field between a `map<K, V>` and the corresponding `repeated` message field is binary compatible (see [Maps](https://developers.google.com/protocol-buffers/docs/proto#maps), below, for the message layout and other restrictions). However, the  safety of the change is application-dependent: when deserializing and  reserializing a message, clients using the `repeated` field definition will produce a semantically identical result; however, clients using the `map` field definition may reorder entries and drop entries with duplicate keys.

## Extensions

扩展允许您声明消息中的一系列字段编号可用于第三方扩展。 扩展名是一个字段的占位符，其类型未由原始.proto文件定义。 这允许其他.proto文件通过定义具有这些字段编号的某些字段的类型来添加到消息定义。 让我们来看看一个例子：

```protobuf
message Foo {
  // ...
  extensions 100 to 199;
}
```

这说明Foo中的字段数[100,199]的范围是为扩展保留的。其他用户现在可以在他们自己的.proto文件中添加新的字段到Foo，以导入您的.proto，例如使用指定范围内的字段编号:

```protobuf
extend Foo {
  optional int32 bar = 126;
}
```

这将具有字段编号126的字段命名栏添加到Foo的原始定义。

当您的用户的FOO消息被编码时，编码格式完全相同，就像用户在Foo内定义了新字段一样。 但是，您在应用程序代码中访问扩展字段的方式与访问常规字段略有不同 - 生成的数据访问代码具有用于与扩展合作的特殊访问器。 因此，例如，这是您如何在C ++中设置bar的值：

```protobuf
Foo foo;
foo.SetExtension(bar, 15);
```

类似地，Foo类定义了模板访问器HasExtension()、ClearExtension()、GetExtension()、MutableExtension()和AddExtension()。它们的语义都与为普通字段生成的相应访问器相匹配。有关使用扩展的更多信息，请参阅所选语言的生成代码参考。

### Nested Extensions

您可以在另一种类型的范围内声明扩展：

```protobuf
message Baz {
  extend Foo {
    optional int32 bar = 126;
  }
  ...
}
```

在这种情况下，要访问此扩展的C ++代码是：

```c++
Foo foo;
foo.SetExtension(Baz::bar, 15);
```

换句话说，唯一的效果就是bar是在Baz的范围内定义的.

这是一个常见的混淆来源:声明嵌套在消息类型中的扩展块并不意味着外部类型和扩展类型之间有任何关系。特别地，上面的例子并不意味着Baz是Foo的任何子类。这意味着符号bar是在Baz的范围内声明的;它只是一个静态成员。

一个常见的模式是在扩展的字段类型的范围内定义扩展。例如，这里有一个类型为Baz的Foo扩展，其中扩展被定义为Baz的一部分

```protobuf
message Baz {
  extend Foo {
    optional Baz foo_ext = 127;
  }
  ...
}
```

但是，不要求在该类型中定义具有消息类型的扩展。你也可以这样做。

```protobuf
message Baz {
  ...
}

// This can even be in a different file.
extend Foo {
  optional Baz foo_baz_ext = 127;
}
```

实际上，最好使用这种语法来避免混淆。如上所述，嵌套语法经常被不熟悉扩展的用户误认为是子类化。

### 选择扩展号码

非常重要的是，要确保两个用户不会使用相同的字段号向相同的消息类型添加扩展，如果扩展被意外地解释为错误的类型，可能会导致数据损坏。您可能需要考虑为您的项目定义一个扩展编号约定，以防止这种情况发生。

如果您的编号约定可能涉及具有非常大的字段编号的扩展，则可以使用MAX关键字指定扩展范围达到最大可能的字段编号：

```protobuf
message Foo {
  extensions 1000 to max;
}
```

`max` is 2^29 - 1, or 536,870,911.

As when choosing field numbers in general, your numbering convention also needs to avoid field numbers 19000 though 19999 (`FieldDescriptor::kFirstReservedNumber` through `FieldDescriptor::kLastReservedNumber`), as they are reserved for the Protocol Buffers implementation. You can  define an extension range that includes this range, but the protocol  compiler will not allow you to define actual extensions with these  numbers.

## Oneof

如果你有一个带有许多可选字段的消息，并且最多将同时设置一个字段，你可以通过使用oneof特性来强制这种行为并节省内存。其中一个字段类似于可选字段，除了一个共享内存中的所有字段，并且最多可以同时设置一个字段。设置oneof中的任何成员将自动清除所有其他成员。您可以使用指定case()或whichoneof()方法检查其中哪个值被设置(如果有的话)，这取决于您选择的语言。

### Using Oneof

要在您的.proto中定义一个oneof您使用Oneof关键字后跟您的Oneof名称，在本例中test_oneof：

```protobuf
message SampleMessage {
  oneof test_oneof {
     string name = 4;
     SubMessage sub_message = 9;
  }
}
```

You then add your oneof fields to the oneof definition. You can add fields of any type, but cannot use the `required`, `optional`, or `repeated` keywords. If you need to add a repeated field to a oneof, you can use a message containing the repeated field.

在生成的代码中，其中一个字段具有与常规可选方法相同的getter和setter。您还可以得到一个特殊的方法来检查设置了哪个值(如果有的话)。您可以在相关的API参考中找到关于所选语言的API的更多信息。

### Oneof Features

设置oneof字段将自动清除oneof的所有其他成员。因此，如果您设置了多个字段，那么只有最后设置的字段仍然有值。

```protobuf
SampleMessage message;
message.set_name("name");
CHECK(message.has_name());
message.mutable_sub_message();   // Will clear name field.
CHECK(!message.has_name());
```

- 如果解析器在线上遇到相同oneof的多个成员，则仅在解析的消息中使用的最后一个成员。
- Oneof不支持扩展。
- A oneof cannot be `repeated`.
- Reflection APIs work for oneof fields.

- 如果将Oneof字段设置为默认值（例如将Int32 Oneof字段设置为0），则将设置该字段的“case”，并且该值将在wire上序列化。

- 如果您使用c++，请确保您的代码不会导致内存崩溃。下面的示例代码将崩溃，因为`sub_message`已经通过调用set name()方法删除了。

  ```protobuf
  SampleMessage message;
  SubMessage* sub_message = message.mutable_sub_message();
  message.set_name("name");      // Will delete sub_message
  sub_message->set_...            // Crashes here
  ```

- Again in C++, if you `Swap()` two messages with oneofs, each message will end up with the other’s oneof case: in the example below, `msg1` will have a `sub_message` and `msg2` will have a `name`.

  ```protobuf
  SampleMessage msg1;
  msg1.set_name("name");
  SampleMessage msg2;
  msg2.mutable_sub_message();
  msg1.swap(&msg2);
  CHECK(msg1.has_sub_message());
  CHECK(msg2.has_name());
  ```

### Backwards-compatibility issues

添加或删除Oneof字段时要小心。 如果检查Oneof的值返回None / Not_set，则可能意味着尚未设置OOF或已将其设置为Oneof的不同版本中的字段。 没有办法讲述差异，因为没有办法知道wire上的未知字段是否是oneof的成员。

#### Tag Reuse Issues

- **Move optional fields into or out of a oneof**: 在序列化和解析消息之后，您可能会丢失一些信息(一些字段将被清除)。但是，您可以安全地将一个字段移动到一个新的字段中，如果已知只设置了一个字段，则可以移动多个字段。
- **Delete a oneof field and add it back**: This may clear your currently set oneof field after the message is serialized and parsed.
- **Split or merge oneof**: This has similar issues to moving regular `optional` fields.

## Maps

如果要将关联地图作为数据定义的一部分创建，协议缓冲区提供了方便的快捷方式语法：

```protobuf
map<key_type, value_type> map_field = N;
```

... key_type可以是任何整体或字符串类型（因此，除浮点类型和字节之外的任何标量类型）。 请注意，ENUM不是有效的key_type。  value_type可以是除其他map之外的任何类型。

因此，例如，如果您想要创建一个项目的项目map，其中每个项目消息与字符串键相关联，则可以如下定义它：

```protobuf
map<string, Project> projects = 3;
```

The generated map API is currently available for all proto2 supported languages. You can find out more about the map API for your chosen  language in the relevant [API reference](https://developers.google.com/protocol-buffers/docs/reference/overview).

### Maps Features

- Extensions are not supported for maps.
- Maps cannot be `repeated`, `optional`, or `required`.
- Wire format ordering and map iteration ordering of map values is  undefined, so you cannot rely on your map items being in a particular  order.
- 当为.proto生成文本格式时，映射是按键排序的。数字键按数字排序。
- 当从连线进行解析或合并时，如果有重复的映射键，则使用最后看到的键。当从文本格式解析映射时，如果有重复的键，解析可能会失败。

### Backwards compatibility

map语法与下面的编码等价，因此不支持映射的协议缓冲区实现仍然可以处理您的数据：

```protobuf
message MapFieldEntry {
  optional key_type key = 1;
  optional value_type value = 2;
}

repeated MapFieldEntry map_field = N;
```

任何支持映射的协议缓冲区实现都必须生成和接受上述定义可以接受的数据。

## Packages

您可以将可选的package说明符添加到.proto文件以防止在协议消息类型之间的名称冲突。

```protobuf
package foo.bar;
message Open { ... }
```

然后，您可以在定义消息类型的字段时使用包说明符：

```protobuf
message Foo {
  ...
  required foo.bar.Open open = 1;
  ...
}
```

包说明符影响生成代码的方式取决于您选择的语言

- In **C++** the generated classes are wrapped inside a C++ namespace. For example, `Open` would be in the namespace `foo::bar`.
- In **Java**, the package is used as the Java package, unless you explicitly provide a `option java_package` in your `.proto` file.
- In **Python**, the `package` directive is ignored, since Python modules are organized according to their location in the file system.
- In **Go**, the `package` directive is ignored, and the generated `.pb.go` file is in the package named after the corresponding `go_proto_library` rule.

请注意，即使包指令不直接影响生成的代码，例如在Python中，仍然强烈建议为.proto文件指定包，否则可能导致描述符中的命名冲突，并使proto不能移植到其他语言。

### Packages and Name Resolution

Type name resolution in the protocol buffer language works like C++:  first the innermost scope is searched, then the next-innermost, and so  on, with each package considered to be "inner" to its parent package. A  leading '.' (for example, `.foo.bar.Baz`) means to start from the outermost scope instead.

The protocol buffer compiler resolves all type names by parsing the imported `.proto` files. The code generator for each language knows how to refer to each  type in that language, even if it has different scoping rules.

## 定义服务

如果你想在RPC(远程过程调用)系统中使用你的消息类型，你可以在.proto文件中定义RPC服务接口，协议缓冲编译器会用你选择的语言生成服务接口代码和存根。因此，例如，如果您想用一个方法定义一个RPC服务，该方法接受您的SearchRequest并返回一个SearchResponse，您可以在.proto文件中如下定义它:

```protobuf
service SearchService {
  rpc Search(SearchRequest) returns (SearchResponse);
}
```

默认情况下，协议编译器将生成一个名为SearchService的抽象接口和一个相应的“stub”实现。stub将所有调用转发到RpcChannel，而RpcChannel又是一个抽象接口，您必须根据自己的RPC系统定义自己。例如，您可以实现一个RpcChannel，它序列化消息并通过HTTP将其发送到服务器。换句话说，生成的stub为进行基于协议缓冲区的RPC调用提供了类型安全的接口，而没有将您锁定在任何特定的RPC实现中。所以，在c++中，你可能会以机智结束:

```c++
using google::protobuf;

protobuf::RpcChannel* channel;
protobuf::RpcController* controller;
SearchService* service;
SearchRequest request;
SearchResponse response;

void DoSearch() {
  // You provide classes MyRpcChannel and MyRpcController, which implement
  // the abstract interfaces protobuf::RpcChannel and protobuf::RpcController.
  channel = new MyRpcChannel("somehost.example.com:1234");
  controller = new MyRpcController;

  // The protocol compiler generates the SearchService class based on the
  // definition given above.
  service = new SearchService::Stub(channel);

  // Set up the request.
  request.set_query("protocol buffers");

  // Execute the RPC.
  service->Search(controller, request, response, protobuf::NewCallback(&Done));
}

void Done() {
  delete service;
  delete channel;
  delete controller;
}
```

所有服务类也都实现了service接口，该接口提供了一种调用特定方法的方法，而不需要知道方法的名称及其在编译时的输入和输出类型。在服务器端，这可以用于实现RPC服务器，您可以用它注册服务。

```c++
using google::protobuf;

class ExampleSearchService : public SearchService {
 public:
  void Search(protobuf::RpcController* controller,
              const SearchRequest* request,
              SearchResponse* response,
              protobuf::Closure* done) {
    if (request->query() == "google") {
      response->add_result()->set_url("http://www.google.com");
    } else if (request->query() == "protocol buffers") {
      response->add_result()->set_url("http://protobuf.googlecode.com");
    }
    done->Run();
  }
};

int main() {
  // You provide class MyRpcServer.  It does not have to implement any
  // particular interface; this is just an example.
  MyRpcServer server;

  protobuf::Service* service = new ExampleSearchService;
  server.ExportOnPort(1234, service);
  server.Run();

  delete service;
  return 0;
}
```

If you don't want to plug in your own existing RPC system, you can now use [gRPC](https://github.com/grpc/grpc-common): a language- and platform-neutral open source RPC system developed at  Google. gRPC works particularly well with protocol buffers and lets you  generate the relevant RPC code directly from your `.proto` files using a special protocol buffer compiler plugin. However, as  there are potential compatibility issues between clients and servers  generated with proto2 and proto3, we recommend that you use proto3 for  defining gRPC services. You can find out more about proto3 syntax in the [Proto3 Language Guide](https://developers.google.com/protocol-buffers/docs/proto3). If you do want to use proto2 with gRPC, you need to use version 3.0.0  or higher of the protocol buffers compiler and libraries.

In addition to gRPC, there are also a number of ongoing third-party  projects to develop RPC implementations for Protocol Buffers. For a list of links to projects we know about, see the [third-party add-ons wiki page](https://github.com/protocolbuffers/protobuf/blob/master/docs/third_party.md).

## Options

Individual declarations in a `.proto` file can be annotated with a number of *options*. Options do not change the overall meaning of a declaration, but may  affect the way it is handled in a particular context. The complete list  of available options is defined in `google/protobuf/descriptor.proto`.

Some options are file-level options, meaning they should be written  at the top-level scope, not inside any message, enum, or service  definition. Some options are message-level options, meaning they should  be written inside message definitions. Some options are field-level  options, meaning they should be written inside field definitions.  Options can also be written on enum types, enum values, oneof fields,  service types, and service methods; however, no useful options currently exist for any of these.

Here are a few of the most commonly used options:

- `java_package` (file option): The package you want to use for your generated Java classes. If no explicit `java_package` option is given in the `.proto` file, then by default the proto package (specified using the "package" keyword in the `.proto` file) will be used. However, proto packages generally do not make good  Java packages since proto packages are not expected to start with  reverse domain names. If not generating Java code, this option has no  effect.

```protobuf
option java_package = "com.example.foo";
```

`java_outer_classname` (file option): The class name (and hence the file name) for the wrapper Java class you want to generate. If no explicit `java_outer_classname` is specified in the `.proto` file, the class name will be constructed by converting the `.proto` file name to camel-case (so `foo_bar.proto` becomes `FooBar.java`). If the `java_multiple_files` option is disabled, then all other classes/enums/etc. generated for the `.proto` file will be generated *within* this outer wrapper Java class as nested classes/enums/etc. If not generating Java code, this option has no effect.

```protobuf
option java_outer_classname = "Ponycopter";
```

- `java_multiple_files` (file option): If false, only a single `.java` file will be generated for this `.proto` file, and all the Java classes/enums/etc. generated for the top-level  messages, services, and enumerations will be nested inside of an outer  class (see `java_outer_classname`). If true, separate `.java` files will be generated for each of the Java classes/enums/etc.  generated for the top-level messages, services, and enumerations, and  the wrapper Java class generated for this `.proto` file won't contain any nested classes/enums/etc. This is a Boolean option which defaults to `false`. If not generating Java code, this option has no effect.

```protobuf
option java_multiple_files = true;
```

- `optimize_for` (file option): Can be set to `SPEED`, `CODE_SIZE`, or `LITE_RUNTIME`. This affects the C++ and Java code generators (and possibly third-party generators) in the following ways:
  - `SPEED` (default): The protocol  buffer compiler will generate code for serializing, parsing, and  performing other common operations on your message types. This code is  highly optimized.
  - `CODE_SIZE`: The protocol buffer compiler will generate minimal classes and will rely on shared,  reflection-based code to implement serialialization, parsing, and  various other operations. The generated code will thus be much smaller  than with `SPEED`, but operations will be slower. Classes will still implement exactly the same public API as they do in `SPEED` mode. This mode is most useful in apps that contain a very large number `.proto` files and do not need all of them to be blindingly fast.
  - `LITE_RUNTIME`: The protocol buffer compiler will generate classes that depend only on the "lite" runtime library (`libprotobuf-lite` instead of `libprotobuf`). The lite runtime is much smaller than the full library (around an order of magnitude smaller) but omits certain features like descriptors and  reflection. This is particularly useful for apps running on constrained  platforms like mobile phones. The compiler will still generate fast  implementations of all methods as it does in `SPEED` mode. Generated classes will only implement the `MessageLite` interface in each language, which provides only a subset of the methods of the full `Message` interface.

```protobuf
option optimize_for = CODE_SIZE;
```

- `cc_generic_services`, `java_generic_services`, `py_generic_services` (file options): Whether or not the protocol buffer compiler should generate abstract service code based on [services definitions](https://developers.google.com/protocol-buffers/docs/proto#services) in C++, Java, and Python, respectively. For legacy reasons, these default to `true`. However, as of version 2.3.0 (January 2010), it is considered preferrable for RPC implementations to provide [code generator plugins](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.compiler.plugin.pb) to generate code more specific to each system, rather than rely on the "abstract" services.

```protobuf
// This file relies on plugins to generate service code.
option cc_generic_services = false;
option java_generic_services = false;
option py_generic_services = false;
```

`cc_enable_arenas` (file option): Enables [arena allocation](https://developers.google.com/protocol-buffers/docs/reference/arenas) for C++ generated code.

- `message_set_wire_format` (message option): If set to `true`, the message uses a different binary format intended to be compatible with an old format used inside Google called `MessageSet`. Users outside Google will probably never need to use this option. The message must be declared exactly as follows:

```protobuf
message Foo {
  option message_set_wire_format = true;
  extensions 4 to max;
}
```

- `packed` (field option): If set to `true` on a repeated field of a basic numeric type, a more compact [encoding](https://developers.google.com/protocol-buffers/docs/encoding#packed) is used. There is no downside to using this option. However, note that  prior to version 2.3.0, parsers that received packed data when not  expected would ignore it. Therefore, it was not possible to change an  existing field to packed format without breaking wire compatibility. In  2.3.0 and later, this change is safe, as parsers for packable fields  will always accept both formats, but be careful if you have to deal with old programs using old protobuf versions.

```protobuf
repeated int32 samples = 4 [packed=true];
```

- `deprecated` (field option): If set to `true`, indicates that the field is deprecated and should not be used by new  code. In most languages this has no actual effect. In Java, this becomes a `@Deprecated` annotation. In the  future, other language-specific code generators may generate deprecation annotations on the field's accessors, which will in turn cause a  warning to be emitted when compiling code which attempts to use the  field. If the field is not used by anyone and you want to prevent new  users from using it, consider replacing the field declaration with a [reserved](https://developers.google.com/protocol-buffers/docs/proto#reserved) statement.

```protobuf
optional int32 old_field = 6 [deprecated=true];
```

### 自定义选项

协议缓冲区甚至允许您定义和使用自己的选项。  请注意，这是大多数人不需要的高级功能。 由于选项由Google / protobuf /  descriptor.proto（如FileOptions或FieldOptions）中定义的消息定义，因此定义自己的选项只是扩展这些消息的问题。 例如：

```protobuf
import "google/protobuf/descriptor.proto";

extend google.protobuf.MessageOptions {
  optional string my_option = 51234;
}

message MyMessage {
  option (my_option) = "Hello world!";
}
```

在这里，我们通过扩展MessageOptions定义了一个新的消息级选项。然后使用该选项时，必须将选项名称括在圆括号中，以表示它是一个扩展。我们现在可以像这样在c++中读取我的选项的值:

```c++
string value = MyMessage::descriptor()->options().GetExtension(my_option);
```

这里，MyMessage::descriptor()->options()返回MyMessage的MessageOptions协议消息。阅读自定义选项从它就像阅读任何其他扩展。

Similarly, in Java we would write:

```java
String value = MyProtoFile.MyMessage.getDescriptor().getOptions() .getExtension(MyProtoFile.myOption);
```

In Python it would be:

```python
value = my_proto_file_pb2.MyMessage.DESCRIPTOR.GetOptions()
  .Extensions[my_proto_file_pb2.my_option]
```

可以为协议缓冲区语言中的每一种构造定义自定义选项。下面是一个使用各种选项的例子:

```protobuf
import "google/protobuf/descriptor.proto";

extend google.protobuf.FileOptions {
  optional string my_file_option = 50000;
}
extend google.protobuf.MessageOptions {
  optional int32 my_message_option = 50001;
}
extend google.protobuf.FieldOptions {
  optional float my_field_option = 50002;
}
extend google.protobuf.OneofOptions {
  optional int64 my_oneof_option = 50003;
}
extend google.protobuf.EnumOptions {
  optional bool my_enum_option = 50004;
}
extend google.protobuf.EnumValueOptions {
  optional uint32 my_enum_value_option = 50005;
}
extend google.protobuf.ServiceOptions {
  optional MyEnum my_service_option = 50006;
}
extend google.protobuf.MethodOptions {
  optional MyMessage my_method_option = 50007;
}

option (my_file_option) = "Hello world!";

message MyMessage {
  option (my_message_option) = 1234;

  optional int32 foo = 1 [(my_field_option) = 4.5];
  optional string bar = 2;
  oneof qux {
    option (my_oneof_option) = 42;

    string quux = 3;
  }
}

enum MyEnum {
  option (my_enum_option) = true;

  FOO = 1 [(my_enum_value_option) = 321];
  BAR = 2;
}

message RequestType {}
message ResponseType {}

service MyService {
  option (my_service_option) = FOO;

  rpc MyMethod(RequestType) returns(ResponseType) {
    // Note:  my_method_option has type MyMessage.  We can set each field
    //   within it using a separate "option" line.
    option (my_method_option).foo = 567;
    option (my_method_option).bar = "Some string";
  }
}
```

请注意，如果要在定义的包中使用自定义选项，则必须使用包名称前缀选项名称，就像您的类型名称一样。 例如：

```protobuf
// foo.proto
import "google/protobuf/descriptor.proto";
package foo;
extend google.protobuf.MessageOptions {
  optional string my_option = 51234;
}
```

```protobuf
// bar.proto
import "foo.proto";
package bar;
message MyMessage {
  option (foo.my_option) = "Hello world!";
}
```

最后一件事：由于自定义选项是扩展，因此必须分配与任何其他字段或扩展名一样的字段编号。 在上面的示例中，我们使用了50000-999999范围内的字段数字。  此范围是保留在单个组织内的内部使用，因此您可以在内部应用程序自由地使用此范围内的数字。  但是，如果您打算在公共应用程序中使用自定义选项，那么重要的是，您必须确保您的字段编号全局唯一。  要获取全局唯一的字段编号，请发送请求以添加进入[protobuf global extension registry](https://github.com/protocolbuffers/protobuf/blob/master/docs/options.md)。 通常您只需要一个分机号码。  您可以通过将它们放入子消息中仅使用一个分机号码声明多个选项：

```protobuf
message FooOptions {
  optional int32 opt1 = 1;
  optional string opt2 = 2;
}

extend google.protobuf.FieldOptions {
  optional FooOptions foo_options = 1234;
}

// usage:
message Bar {
  optional int32 a = 1 [(foo_options).opt1 = 123, (foo_options).opt2 = "baz"];
  // alternative aggregate syntax (uses TextFormat):
  optional int32 b = 2 [(foo_options) = { opt1: 123 opt2: "baz" }];
}
```

此外，请注意，每个选项类型（文件级，消息级，字段等）有其自己的数字空间，因此例如 您可以使用相同的数字声明FieldOptions和MessageOptions的扩展。

## Generating Your Classes

To generate the Java, Python, or C++ code you need to work with the message types defined in a `.proto` file, you need to run the protocol buffer compiler `protoc` on the `.proto`. If you haven't installed the compiler, [download the package](https://developers.google.com/protocol-buffers/docs/downloads) and follow the instructions in the README.

The Protocol Compiler is invoked as follows:

```bash
protoc --proto_path=IMPORT_PATH --cpp_out=DST_DIR --java_out=DST_DIR --python_out=DST_DIR path/to/file.proto
```

- `IMPORT_PATH` specifies a directory in which to look for `.proto` files when resolving `import` directives. If omitted, the current directory is used. Multiple import directories can be specified by passing the `--proto_path` option multiple times; they will be searched in order. `-I=_IMPORT_PATH_` can be used as a short form of `--proto_path`.
- You can provide one or more output directives:
  - `--cpp_out` generates C++ code in `DST_DIR`. See the [C++ generated code reference](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated) for more.
  - `--java_out` generates Java code in `DST_DIR`. See the [Java generated code reference](https://developers.google.com/protocol-buffers/docs/reference/java-generated) for more.
  - `--python_out` generates Python code in `DST_DIR`. See the [Python generated code reference](https://developers.google.com/protocol-buffers/docs/reference/python-generated) for more.As an extra convenience, if the `DST_DIR` ends in `.zip` or `.jar`, the compiler will write the output to a single ZIP-format archive file with the given name. `.jar` outputs will also be given a manifest file as required by the Java JAR  specification. Note that if the output archive already exists, it will  be overwritten; the compiler is not smart enough to add files to an  existing archive.
- You must provide one or more `.proto` files as input. Multiple `.proto` files can be specified at once. Although the files are named relative  to the current directory, each file must reside in one of the `IMPORT_PATH`s so that the compiler can determine its canonical name.



# proto3教程

## 定义消息类型

首先让我们看一下一个非常简单的例子。 假设您希望定义搜索请求消息格式，其中每个搜索请求都有一个查询字符串，您对此感兴趣的结果的特定页面以及每个页面的数量。 以下是您用于定义消息类型的.proto文件

```protobuf
syntax = "proto3";

message SearchRequest {
  string query = 1;
  int32 page_number = 2;
  int32 result_per_page = 3;
}

```

- 文件的第一行指定您使用proto3语法：如果不执行此操作，则协议缓冲区编译器将假设您使用proto2。 这必须是文件的第一个非空的非注释行。

- SearchRequest消息定义指定三个字段（名称/值对），一个用于在此类型的消息中包含的数据。 每个字段都有一个名称和类型。

### 指定字段类型

在上面的示例中，所有字段都是标量类型：两个整数（page_number和result_per_page）和字符串（查询）。 但是，您还可以为字段指定复合类型，包括枚举和其他消息类型。

### 分配字段号码

如您所见，消息定义中的每个字段都有一个惟一的编号。这些字段编号用于以消息二进制格式标识您的字段，一旦使用了消息类型，就不应该更改这些字段编号。注意，1到15范围内的字段编号需要一个字节来编码，包括字段编号和字段的类型(您可以在协议缓冲区编码中了解更多)。16到2047之间的字段数占用两个字节。因此，您应该为非常频繁出现的消息元素保留数字1到15。记得给频繁留点空间

您可以指定的最小字段数为1，最大的是2^29  -  1，或536,870,911。  您也无法使用19000至19999（FieldDescriptor ::  KfirstreservedNumber通过FieldDescriptor ::  KlastreservedNumber），因为它们被保留为协议缓冲区实现，如果您在.proto中使用这些保留的数字之一，则协议缓冲区编译器将抱怨。 同样，您不能使用任何先前保留的字段编号。

### 指定字段规则

消息字段可以是以下之一

- singular: 一个格式良好的消息可以有0个或一个这个字段(但不能多于一个)。这是proto3语法的默认字段规则。

- `repeated`: 该字段可以在形成良好的消息中重复任何次数（包括零）。 重复值的顺序将被保留。

In proto3, `repeated` fields of scalar numeric types use `packed` encoding by default.

### 添加更多消息类型

可以在单个.proto文件中定义多个消息类型。 如果您定义多条消息，这是有用的 - 例如，如果要定义与`SearchResponse`消息类型对应的回复消息格式，则可以将其添加到同一个.proto：

```protobuf
message SearchRequest {
  string query = 1;
  int32 page_number = 2;
  int32 result_per_page = 3;
}

message SearchResponse {
 ...
}
```

### 添加评论

要为您的.proto文件添加注释，请使用c / c ++  -  style //和/ * ... * /语法。

```protobuf
/* SearchRequest represents a search query, with pagination options to
 * indicate which results to include in the response. */

message SearchRequest {
  string query = 1;
  int32 page_number = 2;  // Which page number do we want?
  int32 result_per_page = 3;  // Number of results to return per page.
}
```

### reserved字段

如果通过完全删除字段更新消息类型，或者注释出来，将来可以在对类型进行自己的更新时重复使用字段编号。  如果稍后加载相同的旧版本，这可能会导致严重问题.proto，包括数据损坏，隐私错误等。  确保不会发生这种情况的一种方法是指定要保留删除字段的字段编号（和/或名称，也可能导致JSON序列化问题）。  协议缓冲区编译器如果未来的用户尝试使用这些字段标识符，将发出警告。

```protobuf
message Foo {
  reserved 2, 15, 9 to 11;
  reserved "foo", "bar";
}
```

### 怎生成自己的.proto文件

使用协议缓冲区编译器编译“.proto”，编译器会在所选语言中生成代码，您需要使用文件中描述的消息类型，包括获取和设置字段值，将消息序列化为 输出流，并从输入流解析消息。

- For **C++**, the compiler generates a `.h` and `.cc` file from each `.proto`, with a class for each message type described in your file.
- For **Java**, the compiler generates a `.java` file with a class for each message type, as well as a special `Builder` classes for creating message class instances.
- For **Kotlin**, in addition to the Java generated code, the compiler generates a `.kt` file for each message type, containing a DSL which can be used to simplify creating message instances.
- **Python** is a little different – the Python compiler generates a module with a static descriptor of each message type in your `.proto`, which is then used with a *metaclass* to create the necessary Python data access class at runtime.
- For **Go**, the compiler generates a `.pb.go` file with a type for each message type in your file.
- For **Ruby**, the compiler generates a `.rb` file with a Ruby module containing your message types.
- For **Objective-C**, the compiler generates a `pbobjc.h` and `pbobjc.m` file from each `.proto`, with a class for each message type described in your file.
- For **C#**, the compiler generates a `.cs` file from each `.proto`, with a class for each message type described in your file.
- For **Dart**, the compiler generates a `.pb.dart` file with a class for each message type in your file.

You can find out more about using the APIs for each language by  following the tutorial for your chosen language (proto3 versions coming  soon). For even more API details, see the relevant [API reference](https://developers.google.com/protocol-buffers/docs/reference/overview) (proto3 versions also coming soon).

## 标量值类型

标量消息字段可以具有以下类型之一，表中显示了.proto文件中指定的类型，以及自动生成的类中相应的类型

| .proto Type | Notes                                                        | C++ Type | Java/Kotlin Type[1] | Python Type[3] | Go Type | Ruby Type                      | C# Type    | PHP Type          | Dart Type |
| ----------- | ------------------------------------------------------------ | -------- | ------------------- | -------------- | ------- | ------------------------------ | ---------- | ----------------- | --------- |
| double      |                                                              | double   | double              | float          | float64 | Float                          | double     | float             | double    |
| float       |                                                              | float    | float               | float          | float32 | Float                          | float      | float             | double    |
| int32       | Uses variable-length encoding. Inefficient for encoding negative  numbers – if your field is likely to have negative values, use sint32  instead. | int32    | int                 | int            | int32   | Fixnum or Bignum (as required) | int        | integer           | int       |
| int64       | Uses variable-length encoding. Inefficient for encoding negative  numbers – if your field is likely to have negative values, use sint64  instead. | int64    | long                | int/long[4]    | int64   | Bignum                         | long       | integer/string[6] | Int64     |
| uint32      | Uses variable-length encoding.                               | uint32   | int[2]              | int/long[4]    | uint32  | Fixnum or Bignum (as required) | uint       | integer           | int       |
| uint64      | Uses variable-length encoding.                               | uint64   | long[2]             | int/long[4]    | uint64  | Bignum                         | ulong      | integer/string[6] | Int64     |
| sint32      | Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int32s. | int32    | int                 | int            | int32   | Fixnum or Bignum (as required) | int        | integer           | int       |
| sint64      | Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int64s. | int64    | long                | int/long[4]    | int64   | Bignum                         | long       | integer/string[6] | Int64     |
| fixed32     | Always four bytes. More efficient than uint32 if values are often greater than 228. | uint32   | int[2]              | int/long[4]    | uint32  | Fixnum or Bignum (as required) | uint       | integer           | int       |
| fixed64     | Always eight bytes. More efficient than uint64 if values are often greater than 256. | uint64   | long[2]             | int/long[4]    | uint64  | Bignum                         | ulong      | integer/string[6] | Int64     |
| sfixed32    | Always four bytes.                                           | int32    | int                 | int            | int32   | Fixnum or Bignum (as required) | int        | integer           | int       |
| sfixed64    | Always eight bytes.                                          | int64    | long                | int/long[4]    | int64   | Bignum                         | long       | integer/string[6] | Int64     |
| bool        |                                                              | bool     | boolean             | bool           | bool    | TrueClass/FalseClass           | bool       | boolean           | bool      |
| string      | A string must always contain UTF-8 encoded or 7-bit ASCII text, and cannot be longer than 232. | string   | String              | str/unicode[5] | string  | String (UTF-8)                 | string     | string            | String    |
| bytes       | May contain any arbitrary sequence of bytes no longer than 232. | string   | ByteString          | str            | []byte  | String (ASCII-8BIT)            | ByteString | string            | List      |

您可以了解更多有关在协议缓冲区编码中序列化您的消息时编码这些类型的更多信息。[Protocol Buffer Encoding](https://developers.google.com/protocol-buffers/docs/encoding).

[1] Kotlin uses the corresponding types from Java, even for unsigned types, to ensure compatibility in mixed Java/Kotlin codebases.

[2] In Java, unsigned 32-bit and 64-bit integers are  represented using their signed counterparts, with the top bit simply  being stored in the sign bit.

[3] In all cases, setting values to a field will perform type checking to make sure it is valid.

[4] 64-bit or unsigned 32-bit integers are always  represented as long when decoded, but can be an int if an int is given  when setting the field. In all cases, the value must fit in the type  represented when set. See [2].

[5] Python strings are represented as unicode on decode but can be str if an ASCII string is given (this is subject to change).

[6] Integer is used on 64-bit machines and string is used on 32-bit machines.



## Default Values

解析消息时，如果编码的消息不包含特定的singular元素，则解析对象中的相应字段被设置为该字段的默认值。 这些默认值是特定于类型的：

- For strings, the default value is the empty string.
- For bytes, the default value is empty bytes.
- For bools, the default value is false.
- For numeric types, the default value is zero.
- For [enums](https://developers.google.com/protocol-buffers/docs/proto3#enum), the default value is the **first defined enum value**, which must be 0.
- For message fields, the field is not set. Its exact value is language-dependent. See the [generated code guide](https://developers.google.com/protocol-buffers/docs/reference/overview) for details.

重复字段的默认值为空（通常是相对应语言中的空列表）。

注意,对于标量消息字段,一旦消息解析没有告诉一个字段是否显式地设置为默认值(例如一个布尔值是否设置为false)或不设置:你应该牢记这一点在定义你的消息类型。例如，如果你不希望某些行为在默认情况下也发生，就不要设置一个布尔值false来开启某些行为。还要注意，如果标量消息字段被设置为默认值，则该值将不会在网络上序列化。

See the [generated code guide](https://developers.google.com/protocol-buffers/docs/reference/overview) for your chosen language for more details about how defaults work in generated code.

## 枚举

当您定义消息类型时，您可能希望其中一个字段仅具有预定义的值列表之一。 例如，假设您要为每个搜索查询添加一个Corpus字段，其中语料库可以是通用的，网络，图像，本地，新闻，产品或视频。 您可以非常简单地通过为每个可能的值添加到邮件定义，以常量添加枚举。

```protobuf
message SearchRequest {
  string query = 1;
  int32 page_number = 2;
  int32 result_per_page = 3;
  enum Corpus {
    UNIVERSAL = 0;
    WEB = 1;
    IMAGES = 2;
    LOCAL = 3;
    NEWS = 4;
    PRODUCTS = 5;
    VIDEO = 6;
  }
  Corpus corpus = 4;
}
```

如您所见，Corpus枚举的第一个常量映射到零：每个枚举定义必须包含一个常量映射到零作为其第一个元素。 这是因为：

- There must be a zero value, so that we can use 0 as a numeric [default value](https://developers.google.com/protocol-buffers/docs/proto3#default).
- The zero value needs to be the first element, for compatibility with the [proto2](https://developers.google.com/protocol-buffers/docs/proto) semantics where the first enum value is always the default.

您可以通过为不同的枚举常量分配相同的值来定义别名。 为此，您需要将Allow_Alias选项设置为True，否则协议编译器将在找到别名时生成错误消息。

```protobuf
message MyMessage1 {
  enum EnumAllowingAlias {
    option allow_alias = true;
    UNKNOWN = 0;
    STARTED = 1;
    RUNNING = 1;
  }
}
message MyMessage2 {
  enum EnumNotAllowingAlias {
    UNKNOWN = 0;
    STARTED = 1;
    // RUNNING = 1;  // Uncommenting this line will cause a compile error inside Google and a warning message outside.
  }
}
```

枚举器常量必须在32位整数的范围内。 由于枚举值在线使用Varint编码，因此负值效率低，因此不建议使用。  您可以在消息定义中定义枚举，如上面的示例或外部 - 这些枚举可以在您的.proto文件中的任何消息定义中重用。  您还可以使用一条消息中声明的枚举类型作为不同消息中的字段的类型，使用语法_MessageType _._ enumtype_。

在使用枚举的.proto上运行协议缓冲区编译器时，生成的代码将为java，kotlin或c ++具有相应的枚举，或用于Python的特殊enumdescriptor类，用于创建一组与整数的符号常量 运行时生成的类中的值

在反序列化期间，将在消息中保留未识别的枚举值，但在浏览消息时，如何表示该消息是依赖于语言的。 在支持具有在指定符号范围之外的值的语言，如C  ++和Go之类的值，未知枚举值简单地存储为其基础整数表示。  用诸如Java的封闭枚举类型的语言，枚举中的一个情况用于表示无法识别的值，并且可以使用特殊访问器访问底层整数。  在任何一种情况下，如果消息序列化了无法识别的值仍将使用该消息进行序列化

For more information about how to work with message `enum`s in your applications, see the [generated code guide](https://developers.google.com/protocol-buffers/docs/reference/overview) for your chosen language.

#### Reserved Values

如果通过完全删除枚举输入或注释出来更新枚举类型，则将未来的用户可以在对类型进行自己的更新时重用数值。  如果稍后加载相同的旧版本，这可能会导致严重问题.proto，包括数据损坏，隐私错误等。  确保不会发生这种情况的一种方法是指定要保留删除条目的数值（和/或也可能导致JSON序列化问题的名称）。  协议缓冲区编译器将抱怨如果未来的用户尝试使用这些标识符。 您可以使用MAX关键字指定您的保留数值范围达到最大可能值。

```protobuf
enum Foo {
  reserved 2, 15, 9 to 11, 40 to max;
  reserved "FOO", "BAR";
}
```

请注意，您无法在同一保留语句中混合字段名称和数字值。

## 使用其他消息类型

您可以使用其他消息类型作为字段类型。例如，假设您希望在每个SearchResponse消息中包含Result消息来实现此目的，您可以在相同的.proto中定义一个Result消息类型，然后在SearchResponse中指定一个Result类型的字段

```protobuf
message SearchResponse {
  repeated Result results = 1;
}

message Result {
  string url = 1;
  string title = 2;
  repeated string snippets = 3;
}
```

### 导入定义

**注意，此特性在Java中不可用。**

在上面的例子中，Result消息类型定义在与SearchResponse相同的文件中，如果您想要作为字段类型使用的消息类型已经在另一个.proto文件中定义了该怎么办

您可以通过导入它们来使用其他.proto文件的定义。 要导入另一个.proto的定义，请将导入语句添加到文件顶部：

```protobuf
import "myproject/other_protos.proto";
```

默认情况下，您只能使用直接导入的.proto文件中的定义。但是，有时您可能需要将.proto文件移动到新位置。与直接移动.proto文件并在单个更改中更新所有调用站点不同，现在您可以在旧位置放置一个虚拟的.proto文件，以使用import public概念将所有导入转发到新位置。任何导入包含Import public语句的原型的人都可以传递地依赖于导入公共依赖项。

```protobuf
// new.proto
// All definitions are moved here
```

```protobuf
// old.proto
// This is the proto that all clients are importing.
import public "new.proto";
import "other.proto";
```

```protobuf
// client.proto
import "old.proto";
// You use definitions from old.proto and new.proto, but not other.proto
```

协议编译器在协议编译器命令行中指定的一组目录中搜索导入的文件使用-i /  -  proto_path标志。 如果没有给出任何标志，它会在调用编译器的目录中。 通常，您应该将-proto_path标志设置为项目的根目录，并为所有导入使用完全限定的名称。

### 使用proto2消息类型

可以导入proto2消息类型并在proto3消息中使用它们，反之亦然。然而，proto2枚举不能在proto3语法中直接使用(如果导入的proto2消息使用它们也没关系)。

## 嵌套类型

您可以在其他消息类型中定义和使用消息类型，如在以下示例中 - 此处在SearchResponse消息中定义了结果消息：

```protobuf
message SearchResponse {
  message Result {
    string url = 1;
    string title = 2;
    repeated string snippets = 3;
  }
  repeated Result results = 1;
}
```

如果要在其父消息类型之外重用此消息类型，则将其称为_parent _._ type_：

```protobuf
message SomeOtherMessage {
  SearchResponse.Result result = 1;
}
```

您可以随心所欲地嵌套消息

```protobuf
message Outer {                  // Level 0
  message MiddleAA {  // Level 1
    message Inner {   // Level 2
      int64 ival = 1;
      bool  booly = 2;
    }
  }
  message MiddleBB {  // Level 1
    message Inner {   // Level 2
      int32 ival = 1;
      bool  booly = 2;
    }
  }
}
```

## 更新消息类型

如果现有的消息类型不再符合所有需求 - 例如，您希望消息格式有额外的字段 - 但您仍然希望使用旧格式创建的代码，不要担心！ 在不打破任何现有代码的情况下更新消息类型非常简单。 只需记住以下规则：

- 不要更改任何现有字段的字段编号。

- 如果添加新字段，则仍可使用您的“旧”消息格式按代码序列化的任何消息仍然可以由新生成的代码解析。 您应该记住这些元素的默认值，以便新代码可以与旧代码生成的消息正确交互。  类似地，您的旧代码可以解析由新代码创建的消息：旧的二进制文件在解析时忽略新字段。 有关详细信息，请参阅未知字段部分。
- 可以删除字段，只要在更新的消息类型中不再使用字段编号。您可能想要重命名该字段，比如添加前缀“OBSOLETE”，或者保留字段编号，以便.proto的未来用户不会意外地重用该编号。
- INT32，UINT32，INT64，UINT64和BOOL都是兼容的 - 这意味着您可以将这些类型之一的字段更改为另一个字段，而不会破坏前进或向后兼容性。  如果从不适应相应类型的线路解析的数字，则会获得相同的效果，就像将数字投用到C ++中的类型（例如，如果64位数读为INT32）  它将被截断为32位）。
- SINT32和SINT64彼此兼容，但与其他整数类型不兼容。
- 只要字节是有效的UTF-8，字符串和字节兼容。
- 如果字节包含消息的编码版本，则嵌入式消息与字节兼容。
- Fixed32与SFixED32兼容，SFIXED64与SFIXED64兼容。
- 对于字符串、字节和消息字段，optional和repeated是兼容的。给定一个重复字段的序列化数据作为输入，期望该字段是可选的客户端将接受最后一个输入值(如果它是一个原始类型字段)，或者合并所有输入元素(如果它是一个消息类型字段)。注意，对于数值类型，包括bool和enum，这通常是不安全的。数值类型的重复字段可以以打包格式序列化，如果需要一个可选字段，则不能正确解析该格式。
- Enum兼容int32、uint32、int64和uint64的wire格式(注意如果不匹配会被截断)。但是，请注意，当消息被反序列化时，客户端代码可能会以不同的方式对待它们:例如，未被识别的proto3 enum类型将在消息中保留，但是在消息被反序列化时如何表示这些类型取决于语言。Int字段总是保留它们的值。

- 将一个值更改为一个新值的成员是安全的，并且与二进制兼容。如果您确定一次没有将代码设置于多个字段，那么将多个字段移动到一个新的字段中是安全的。将任何字段移动到现有字段中都是不安全的。

## 未知的字段

未知字段是格式良好的协议缓冲区序列化数据，表示解析器不能识别的字段。例如，当一个旧二进制文件解析带有新字段的新二进制文件发送的数据时，这些新字段在旧二进制文件中变成未知字段。

最初，proto3消息总是在解析过程中丢弃未知字段，但在3.5版本中，我们重新引入了保留未知字段以匹配proto2的行为。在3.5及更高版本中，在解析过程中保留未知字段，并将其包含在序列化输出中。

## Any

任何消息类型都允许您使用消息作为嵌入类型而不具有它们的.proto定义。URL最为该字段的全局标识，并解析该消息类型 。 要使用Any类型，您需要导入Google / Protobuf / Any.proto。

```protobuf
import "google/protobuf/any.proto";

message ErrorStatus {
  string message = 1;
  repeated google.protobuf.Any details = 2;
}
```

给定消息类型的默认类型URL为type.googleapis.com/_packageName_._messageName_。

不同的语言实现将支持运行时库助手以类型安全的方式打包和解包Any值。例如，在Java中，Any类型将有特殊的pack()和unpack()访问器，而在c++中有PackFrom()和UnpackTo()方法

```c++
// Storing an arbitrary message type in Any.
NetworkErrorDetails details = ...;
ErrorStatus status;
status.add_details()->PackFrom(details);

// Reading an arbitrary message from Any.
ErrorStatus status = ...;
for (const Any& detail : status.details()) {
  if (detail.Is<NetworkErrorDetails>()) {
    NetworkErrorDetails network_error;
    detail.UnpackTo(&network_error);
    ... processing network_error ...
  }
}
```

目前，用于处理Any类型的运行时库正在开发中。

如果您已经熟悉proto2语法，Any可以保存任意的proto3消息，类似于proto2消息可以允许扩展。

## Oneof

如果你有一个包含很多字段的消息，并且在同一时间最多设置一个字段，你可以通过使用其中一个特性来强制这种行为并节省内存。

除了共享内存中的所有字段外，其中一个字段与常规字段类似，并且最多可以同时设置一个字段。设置oneof中的任何成员将自动清除所有其他成员。您可以使用case()或whichoneof()方法检查其中哪个值被设置(如果有的话)，这取决于您选择的语言。

### Using Oneof

要在.proto中定义oneof，使用oneof关键字后跟oneof名称，在本例中test_oneof:

```protobuf
message SampleMessage {
  oneof test_oneof {
    string name = 4;
    SubMessage sub_message = 9;
  }
}
```

然后将oneof字段添加到oneof definition。您可以添加任何类型的字段，除了repeated字段和map字段。

在生成的代码中，其中一个字段具有与常规字段相同的getter和setter。您还可以得到一个特殊的方法来检查设置了哪个值(如果有的话)。您可以在相关的API参考中找到关于所选语言的API的更多信息。

### Oneof Features

- 设置oneof字段将自动清除oneof的所有其他成员。因此，如果您设置了多个字段，那么只有最后设置的字段将覆盖前面的设置。

  ```protobuf
  SampleMessage message;
  message.set_name("name");
  CHECK(message.has_name());
  message.mutable_sub_message();   // Will clear name field.
  CHECK(!message.has_name());
  ```

- 如果解析器在网络上遇到同一个成员的多个成员，则在已解析的消息中只使用最后看到的成员。

- A oneof cannot be `repeated`.

- Reflection APIs work for oneof fields.
- 如果您将oneof字段设置为默认值(例如将int32 oneof字段设置为0)，该字段的“case”将被设置，并且该值将在网络上序列化。

- 如果您使用c++，请确保您的代码不会导致内存崩溃。下面的示例代码将崩溃，因为sub_message已经通过调用set_name()方法删除了。

- 再次在C ++中，如果Swap()两个具有oneof的消息，每条消息将结束其他的Oneof Case：在下面的示例中，n the example below, `msg1` will have a `sub_message` and `msg2` will have a `name`.

  ```c++
  SampleMessage msg1;
  msg1.set_name("name");
  SampleMessage msg2;
  msg2.mutable_sub_message();
  msg1.swap(&msg2);
  CHECK(msg1.has_sub_message());
  CHECK(msg2.has_name());
  ```

### Backwards-compatibility issues

 添加或删除某个字段时要小心。如果检查oneof的值返回None/NOT_SET，则可能意味着尚未设置oneof或已将其设置为oneof的其他版本中的字段。因为无法知道wire上的未知字段是否是其中一个字段的成员，所以无法区分两者之间的区别。 

#### 标签重用问题

- 将字段移到或移出oneof字段:在消息被序列化和解析后，您可能会丢失一些信息(一些字段将被清除)。但是，您可以安全地将一个字段移动到一个新的字段中，如果已知只设置了一个字段，则可以移动多个字段。

- 删除一个Oneof字段并随后添加它：这可能会在序列化并解析消息后清除当前设置的Oneof字段。

- 拆分或合并Oneof：这对移动常规字段具有类似的问题。

## Maps

如果您想要创建一个关联映射作为数据定义的一部分，protocol buffers提供了一个方便的快捷语法

```protobuf
map<key_type, value_type> map_field = N;
```

... key_type可以是任何整体或字符串类型（因此，除浮点类型和字节之外的任何标量类型）。 请注意，enum不是有效的key_type。  value_type可以是除其他map之外的任何类型。

因此，例如，如果您想创建一个项目映射，其中每个Project消息都与一个字符串键相关联，您可以这样定义它:

```protobuf
map<string, Project> projects = 3;
```

- Map fields cannot be `repeated`.
- Wire format ordering and map iteration ordering of map values is  undefined, so you cannot rely on your map items being in a particular  order.
- 当为.proto生成文本格式时，映射是按键排序的。数字键按数字排序。
- 当从wire进行解析或合并时，如果有重复的映射键，则使用最后看到的键。当从文本格式解析映射时，如果有重复的键，解析可能会失败。
- 如果为map字段提供了键但没有值，则该字段序列化时的行为与语言有关。在c++、Java、Kotlin和Python中，该类型的默认值是序列化的，而在其他语言中没有序列化。

The generated map API is currently available for all proto3 supported languages. You can find out more about the map API for your chosen  language in the relevant [API reference](https://developers.google.com/protocol-buffers/docs/reference/overview).

### 向后兼容

map语法与下面的连接等价，因此不支持映射的协议缓冲区实现仍然可以处理您的数据

```protobuf
message MapFieldEntry {
  key_type key = 1;
  value_type value = 2;
}

repeated MapFieldEntry map_field = N;
```

任何支持映射的协议缓冲区实现都必须生成和解析上述定义可以接受的数据。

## Packages

您可以向.proto文件添加一个可选的package说明符，以防止协议消息类型之间的名称冲突。

```protobuf
package foo.bar;
message Open { ... }
```

然后，您可以在定义消息类型的字段时使用包说明符:

```protobuf
message Foo {
  ...
  foo.bar.Open open = 1;
  ...
}
```

包说明符影响生成代码的方式取决于您选择的语言:

- In **C++** the generated classes are wrapped inside a C++ namespace. For example, `Open` would be in the namespace `foo::bar`.
- In **Java** and **Kotlin**, the package is used as the Java package, unless you explicitly provide an `option java_package` in your `.proto` file.
- In **Python**, the package directive is ignored, since Python modules are organized according to their location in the file system.
- In **Go**, the package is used as the Go package name, unless you explicitly provide an `option go_package` in your `.proto` file.
- In **Ruby**, the generated classes are wrapped inside  nested Ruby namespaces, converted to the required Ruby capitalization  style (first letter capitalized; if the first character is not a letter, `PB_` is prepended). For example, `Open` would be in the namespace `Foo::Bar`.
- In **C#** the package is used as the namespace after converting to PascalCase, unless you explicitly provide an `option csharp_namespace` in your `.proto` file. For example, `Open` would be in the namespace `Foo.Bar`.

### Packages and Name Resolution

协议缓冲区语言中的类型名称解析的工作原理与c++类似:首先搜索最内层的作用域，然后搜索下一个最内层的作用域，以此类推，每个包都被认为是父包的“内部”。一个最前的"."(例如，.foo.bar. baz)表示从最外层作用域开始。

协议缓冲编译器通过解析导入的.proto文件来解析所有类型名。每种语言的代码生成器知道如何引用该语言中的每种类型，即使它有不同的作用域规则。

## Defining Services

如果你想在RPC(远程过程调用)系统中使用你的消息类型，你可以在.proto文件中定义RPC服务接口，协议缓冲编译器会用你选择的语言生成服务接口代码和stubs。因此，例如，如果您想用一个方法定义一个RPC服务，该方法接受您的SearchRequest并返回一个SearchResponse，您可以在.proto文件中如下定义它

```protobuf
service SearchService {
  rpc Search(SearchRequest) returns (SearchResponse);
}
```

与协议缓冲区一起使用的最直接的RPC系统是 [gRPC](https://grpc.io):一个在谷歌开发的语言和平台无关的开源RPC系统。gRPC在协议缓冲区方面工作得特别好，它允许你使用一个特殊的协议缓冲区编译器插件从.proto文件中直接生成相关的RPC代码。

如果您不想使用GRPC，也可以使用自己的RPC实现使用协议缓冲区。 您可以在[Proto2 Language Guide](https://developers.google.com/protocol-buffers/docs/proto#services)中了解更多信息。

还有许多正在进行的第三方项目为协议缓冲区开发RPC实现。有关我们所知道的项目的链接列表，请参阅[third-party add-ons wiki page](https://github.com/protocolbuffers/protobuf/blob/master/docs/third_party.md).

## JSON Mapping

Proto3支持JSON中的规范编码，使系统之间的数据更容易。 编码在下表中以逐种类型的基础描述。

如果在JSON编码的数据中缺少值，或者其值为null，则将其解释为当解析到协议缓冲区时的适当默认值。 如果字段在协议缓冲区中具有默认值，则默认情况下将在JSON编码的数据中省略它以节省空间。 实现可以提供具有JSON编码输出中的默认值的字段的选项。

| proto3                 | JSON          | JSON example                              | Notes                                                        |
| ---------------------- | ------------- | ----------------------------------------- | ------------------------------------------------------------ |
| message                | object        | `{"fooBar": v, "g": null, …}`             | Generates JSON objects. Message field names are mapped to lowerCamelCase and become JSON object keys. If the `json_name` field option is specified, the specified value will be used as the key  instead. Parsers accept both the lowerCamelCase name (or the one  specified by the `json_name` option) and the original proto field name. `null` is an accepted value for all field types and treated as the default value of the corresponding field type. |
| enum                   | string        | `"FOO_BAR"`                               | The name of the enum value as specified in proto is used. Parsers accept both enum names and integer values. |
| map<K,V>               | object        | `{"k": v, …}`                             | All keys are converted to strings.                           |
| repeated V             | array         | `[v, …]`                                  | `null` is accepted as the empty list `[]`.                   |
| bool                   | true, false   | `true, false`                             |                                                              |
| string                 | string        | `"Hello World!"`                          |                                                              |
| bytes                  | base64 string | `"YWJjMTIzIT8kKiYoKSctPUB+"`              | JSON value will be the data encoded as a string using standard  base64 encoding with paddings. Either standard or URL-safe base64  encoding with/without paddings are accepted. |
| int32, fixed32, uint32 | number        | `1, -10, 0`                               | JSON value will be a decimal number. Either numbers or strings are accepted. |
| int64, fixed64, uint64 | string        | `"1", "-10"`                              | JSON value will be a decimal string. Either numbers or strings are accepted. |
| float, double          | number        | `1.1, -10.0, 0, "NaN", "Infinity"`        | JSON value will be a number or one of the special string values  "NaN", "Infinity", and "-Infinity". Either numbers or strings are  accepted. Exponent notation is also accepted.  -0 is considered  equivalent to 0. |
| Any                    | `object`      | `{"@type": "url", "f": v, … }`            | If the Any contains a value that has a special JSON mapping, it will be converted as follows: `{"@type": xxx, "value": yyy}`. Otherwise, the value will be converted into a JSON object, and the `"@type"` field will be inserted to indicate the actual data type. |
| Timestamp              | string        | `"1972-01-01T10:00:20.021Z"`              | Uses RFC 3339, where generated output will always be Z-normalized  and uses 0, 3, 6 or 9 fractional digits. Offsets other than "Z" are also accepted. |
| Duration               | string        | `"1.000340012s", "1s"`                    | Generated output always contains 0, 3, 6, or 9 fractional digits,  depending on required precision, followed by the suffix "s". Accepted  are any fractional digits (also none) as long as they fit into  nano-seconds precision and the suffix "s" is required. |
| Struct                 | `object`      | `{ … }`                                   | Any JSON object. See `struct.proto`.                         |
| Wrapper types          | various types | `2, "2", "foo", true, "true", null, 0, …` | Wrappers use the same representation in JSON as the wrapped primitive type, except that `null` is allowed and preserved during data conversion and transfer. |
| FieldMask              | string        | `"f.fooBar,h"`                            | See `field_mask.proto`.                                      |
| ListValue              | array         | `[foo, bar, …]`                           |                                                              |
| Value                  | value         |                                           | Any JSON value. Check [google.protobuf.Value](https://developers.google.com/protocol-buffers/docs/reference/google.protobuf#google.protobuf.Value) for details. |
| NullValue              | null          |                                           | JSON null                                                    |
| Empty                  | object        | `{}`                                      | An empty JSON object                                         |

### JSON options

Proto3 JSON实现可以提供以下选项：

- **生成带有默认值的字段:**在proto3 JSON输出中，带有默认值的字段默认被省略。实现可以提供一个选项，用默认值覆盖此行为和输出字段。
- Proto3 JSON解析器在默认情况下应该拒绝未知字段，但可能会提供一个选项，在解析中忽略未知字段。
- **Use proto field name instead of lowerCamelCase name**: By default proto3 JSON printer should convert the field name to  lowerCamelCase and use that as the JSON name. An implementation may  provide an option to use proto field name as the JSON name instead.  Proto3 JSON parsers are required to accept both the converted  lowerCamelCase name and the proto field name.
- **Emit enum values as integers instead of strings**:  在JSON输出中默认使用枚举值的名称。可以提供一个选项来使用枚举值的数值代替。

## Options

可以使用多个*options*注释.proto文件中的单个声明。 选项不会更改声明的整体含义，但可能会影响其在特定上下文中处理的方式。 可用选项的完整列表在`google/protobuf/descriptor.proto`中定义。

有些选项是文件级的选项，这意味着它们应该写在顶层作用域中，而不是在任何消息、枚举或服务定义中。有些选项是消息级别的选项，这意味着它们应该写在消息定义内部。有些选项是字段级选项，这意味着它们应该写在字段定义中。选项也可以写枚举类型，枚举值，字段之一，服务类型和服务方法; 但目前还没有任何有用的选项。

以下是一些最常用的选项：

- `java_package` (file option): 您要为生成的Java / Kotlin类使用的包。  如果在.proto文件中没有给出了显式java_package选项，则默认默认为proto包（使用“.proto文件”中使用“Package”关键字）。 但是，Proto封装通常不会制作良好的Java软件包，因为PROTO软件包预计不会以反向域名开头。  如果没有生成Java或Kotlin代码，则此选项无效。

```protobuf
option java_package = "com.example.foo";
```

- `java_outer_classname` (file option): The class name (and hence the file name) for the wrapper Java class you want to generate. If no explicit `java_outer_classname` is specified in the `.proto` file, the class name will be constructed by converting the `.proto` file name to camel-case (so `foo_bar.proto` becomes `FooBar.java`). If the `java_multiple_files` option is disabled, then all other classes/enums/etc. generated for the `.proto` file will be generated *within* this outer wrapper Java class as nested classes/enums/etc. If not generating Java code, this option has no effect.

```protobuf
option java_outer_classname = "Ponycopter";
```

- `java_multiple_files` (file option): If false, only a single `.java` file will be generated for this `.proto` file, and all the Java classes/enums/etc. generated for the top-level  messages, services, and enumerations will be nested inside of an outer  class (see `java_outer_classname`). If true, separate `.java` files will be generated for each of the Java classes/enums/etc.  generated for the top-level messages, services, and enumerations, and  the wrapper Java class generated for this `.proto` file won't contain any nested classes/enums/etc. This is a Boolean option which defaults to `false`. If not generating Java code, this option has no effect.

```protobuf
option java_multiple_files = true;
```

- `optimize_for` (file option): Can be set to `SPEED`, `CODE_SIZE`, or `LITE_RUNTIME`. This affects the C++ and Java code generators (and possibly third-party generators) in the following ways:

  - `SPEED` (default): 协议缓冲编译器将生成用于对消息类型进行序列化、解析和执行其他常见操作的代码。这段代码是高度优化的。

  - `CODE_SIZE`: 协议缓冲编译器将生成最小的类，并依赖共享的、基于反射的代码来实现序列化、解析和各种其他操作。因此，生成的代码将比使用SPEED时小得多，但操作将会更慢。类仍然将实现与在SPEED模式下完全相同的公共API。这种模式在包含大量.proto文件的应用程序中非常有用，并且不需要所有的.proto文件都非常快。

  - `LITE_RUNTIME`: 协议缓冲区编译器将生成仅依赖于“Lite”运行时库（libprotobuf-lite而不是libprotobuf）的类。   Lite运行时比完整库小得多（距较小的数量级），但省略了描述符和反射等特征。 这对于在移动电话等约束平台上运行的应用特别有用。  编译器仍将在速度模式下产生所有方法的快速实现。 生成的类只会在每种语言中实现Messagelite接口，它仅提供完整消息界面的方法的子集。

```protobuf
option optimize_for = CODE_SIZE;
```

- `cc_enable_arenas` (file option): Enables [arena allocation](https://developers.google.com/protocol-buffers/docs/reference/arenas) for C++ generated code.

- `objc_class_prefix` (file  option): 设置Objective-C类前缀，该前缀是所有Objective-C生成的类和从.proto中生成的枚举的前缀。没有默认值。你应该使用苹果推荐的3-5个大写字母之间的前缀。请注意，所有两个字母的前缀都是苹果保留的。

- `deprecated` (field option): 如果设置为true，表示该字段已弃用，不应被新代码使用。在大多数语言中，这没有实际效果。在Java中，这变成了@Deprecated注释。将来，其他特定于语言的代码生成器可能会在字段的访问器上生成弃用注释，这反过来会在编译试图使用该字段的代码时发出警告。如果该字段未被任何人使用，并且您希望阻止新用户使用它，请考虑使用保留语句替换该字段声明。

```protobuf
int32 old_field = 6 [deprecated = true];
```

#### 自定义选项

协议缓冲区还允许您定义和使用自己的选项。 这是大多数人不需要的高级功能。 如果您认为，您需要创建自己的选项，请参阅ProTo2语言指南以获取详细信息。 请注意，创建自定义选项使用扩展名，仅在Proto3中允许的自定义选项。

## Generating Your Classes

要生成Java，Kotlin，Python，C  ++，Go，Ruby，Objective-C或C＃代码您需要使用.proto文件中定义的消息类型，您需要在.proto上运行协议缓冲区编译器protoc  。 如果您尚未安装编译器，请[download the package](https://developers.google.com/protocol-buffers/docs/downloads) 并按照README中的说明进行操作。  对于Go，您还需要为编译器安装特殊的代码生成器插件：您可以在GitHub上找到 [golang/protobuf](https://github.com/golang/protobuf/)存储库中的此产品的安装说明。

协议编译器被调用如下：

```protobuf
protoc --proto_path=IMPORT_PATH --cpp_out=DST_DIR --java_out=DST_DIR --python_out=DST_DIR --go_out=DST_DIR --ruby_out=DST_DIR --objc_out=DST_DIR --csharp_out=DST_DIR path/to/file.proto
```

- `IMPORT_PATH` 指定在解析导入指令时查找.proto文件的目录。如果省略，则使用当前目录。可以通过多次传递`--proto_path`选项来指定多个导入目录;他们将被依次搜查。-I= IMPORT PATH可以用作`--proto_path`的缩写形式。
- 您可以提供一个或多个输出指令：
  - `--cpp_out` generates C++ code in `DST_DIR`. See the [C++ generated code reference](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated) for more.
  - `--java_out` generates Java code in `DST_DIR`. See the [Java generated code reference](https://developers.google.com/protocol-buffers/docs/reference/java-generated) for more.
  - `--kotlin_out` generates additional Kotlin code in `DST_DIR`. See the [Kotlin generated code reference](https://developers.google.com/protocol-buffers/docs/reference/kotlin-generated) for more.
  - `--python_out` generates Python code in `DST_DIR`. See the [Python generated code reference](https://developers.google.com/protocol-buffers/docs/reference/python-generated) for more.
  - `--go_out` generates Go code in `DST_DIR`. See the [Go generated code reference](https://developers.google.com/protocol-buffers/docs/reference/go-generated) for more.
  - `--ruby_out` generates Ruby code in `DST_DIR`. Ruby generated code reference is coming soon!
  - `--objc_out` generates Objective-C code in `DST_DIR`. See the [Objective-C generated code reference](https://developers.google.com/protocol-buffers/docs/reference/objective-c-generated) for more.
  - `--csharp_out` generates C# code in `DST_DIR`. See the [C# generated code reference](https://developers.google.com/protocol-buffers/docs/reference/csharp-generated) for more.
  - `--php_out` generates PHP code in `DST_DIR`. See the [PHP generated code reference](https://developers.google.com/protocol-buffers/docs/reference/php-generated) for more.As an extra convenience, if the `DST_DIR` ends in `.zip` or `.jar`, the compiler will write the output to a single ZIP-format archive file with the given name. `.jar` outputs will also be given a manifest file as required by the Java JAR  specification. Note that if the output archive already exists, it will  be overwritten; the compiler is not smart enough to add files to an  existing archive.
- 您必须提供一个或多个.proto文件作为输入。可以一次指定多个.proto文件。尽管文件的命名相对于当前目录，但每个文件必须驻留在`IMPORT_PATH`s中的一个中，以便编译器可以确定其规范名称。



# Style Guide

本文档提供了.proto文件的样式指南。通过遵循这些约定，您将使协议缓冲区消息定义及其相应的类保持一致并易于阅读。

请注意，协议缓冲区样式随着时间的推移而发展，因此您可能会看到以不同的约定或样式编写的proto文件。 修改这些文件时，请尊重现有风格。 一致性是关键。 但是，当您创建新的，最佳样式最好采用当前的最佳样式。

## 标准文件格式化

- Keep the line length to 80 characters.
- Use an indent of 2 spaces.
- Prefer the use of double quotes for strings.

## 文件结构

Files should be named `lower_snake_case.proto`

所有文件应该遵守下面的规则：

- License header (if applicable)

- File overview

- Syntax

- Package

- Imports (sorted)

- File options

- Everything else

## Packages

包名称应小写，并且应对应于目录层次结构。 例如，如果文件在`my/package/`, 则包名称应该是my.package。

## 消息和字段名称

使用CamelCase(带有首字母)作为消息名，例如SongServerRequest。使用下划线分隔字段名(包括字段名和扩展名中的一个)，例如，song_name。

```protobuf
message SongServerRequest {
  optional string song_name = 1;
}
```

对字段名使用这种命名约定将为您提供如下访问器

```c++
C++:
  const string& song_name() { ... }
  void set_song_name(const string& x) { ... }

Java:
  public String getSongName() { ... }
  public Builder setSongName(String v) { ... }
```

如果您的字段名称包含一个数字，则该号码应显示在字母后而不是下划线之后。 例如，使用song_name1而不是song_name_1

## 重复的字段

对重复的字段使用复数名称.

```protobuf
  repeated string keys = 1;
  ...
  repeated MyMessage accounts = 17;
```

## 枚举

枚举类型名称使用CamelCase(带有首字母大写)，值名称使用CAPITALS_WITH_UNDERSCORES

```protobuf
enum FooBar {
  FOO_BAR_UNSPECIFIED = 0;
  FOO_BAR_FIRST_VALUE = 1;
  FOO_BAR_SECOND_VALUE = 2;
}
```

每个枚举值应该以分号结尾，而不是逗号。使用前缀枚举值，而不是将它们包围在一个封闭消息中。零值枚举应该有未指定的后缀。

## Services

如果您的.proto定义了RPC服务，您应该使用CamelCase(带有初始大写)作为服务名和任何RPC方法名

```protobuf
service FooService {
  rpc GetSomething(FooRequest) returns (FooResponse);
}
```

## 要避免的事情

- Required fields (only for proto2)
- Groups (only for proto2)





# Encoding

本文档描述了协议缓冲区消息的二进制线格式。 您无需了解此应用程序在应用程序中使用协议缓冲区，但了解不同的协议缓冲区格式会影响编码消息的大小非常有用。

## A Simple Message

假设您有以下非常简单的消息定义：

```protobuf
message Test1 {
  optional int32 a = 1;
}
```

在应用程序中，创建Test1消息并将a设置为150。然后将消息序列化到输出流。如果您能够检查已编码的消息，您将看到三个字节

```bash
08 96 01
```

到目前为止，这个数字很小，但它意味着什么呢?继续阅读…

## Base 128 Varints

要理解简单的协议缓冲区编码，首先需要理解varints。varint是一种使用一个或多个字节序列化整数的方法。较小的数字占用较小的字节数。

varint中的每个字节，除了最后一个字节，都设置了最高有效位(msb)，这表明还有更多的字节要来。每个字节的低7位用于存储7位组中数字的两个补码表示，最低有效组优先。

例如，这是数字1它是一个单字节，所以msb没有设置

```bash
0000 0001
```

And here is 300 – this is a bit more complicated:

```bash
1010 1100 0000 0010
```

你怎么知道这是300？ 首先，从每个字节中删除MSB，因为这是在那里告诉我们我们是否已到达号码的末尾（如您所见，它在第一个字节中设置为varint中有多个字节） ：

```bash
 1010 1100 0000 0010
→ 010 1100  000 0010
```

你颠倒这两组7位，因为，正如你所记得的，varints首先存储的是最低有效组的数字。然后将它们连接起来以获得最终值:

```bash
000 0010  010 1100
→  000 0010 ++ 010 1100
→  100101100
→  256 + 32 + 8 + 4 = 300
```

## Message Structure

如您所知，协议缓冲区消息是一系列键-值对。二进制版本的消息只使用字段的编号作为键值，每个字段的名称和声明的类型只能在解码端通过引用消息类型的定义(即.proto文件)来确定。

当对消息进行编码时，键和值将被连接到字节流中。 当邮件被解码时，解析器需要能够跳过它无法识别的字段。  这样，可以将新字段添加到消息中而不破坏不了解它们的旧程序。 为此，在wire-format 消息中每对的“键”实际上是两个值 —— .proto文件中的字段编号，以及提供足够信息的*wire*类型以确定值的长度。 在大多数语言实现中，此键被称为标签。

The available wire types are as follows:

| Type | Meaning          | Used For                                                 |
| ---- | ---------------- | -------------------------------------------------------- |
| 0    | Varint           | int32, int64, uint32, uint64, sint32, sint64, bool, enum |
| 1    | 64-bit           | fixed64, sfixed64, double                                |
| 2    | Length-delimited | string, bytes, embedded messages, packed repeated fields |
| 3    | Start group      | groups (deprecated)                                      |
| 4    | End group        | groups (deprecated)                                      |
| 5    | 32-bit           | fixed32, sfixed32, float                                 |

流消息中的每个键都是一个varint值(field_number << 3) | wire_type，换句话说，编号的最后三位存储wire类型。

现在让我们再次看看我们的简单例子。现在您知道流中的第一个数字总是一个varint键，这里是08，或者(去掉msb):

```bash
000 1000
```

您取最后三位以获得wire类型(0)，然后右移3位以获得字段号(1)。所以现在您知道字段号是1，下面的值是一个varint。使用上一节中的varint解码知识，您可以看到接下来的两个字节存储了值150。

```bash
96 01 = 1001 0110  0000 0001
       → 000 0001  ++  001 0110 (drop the msb and reverse the groups of 7 bits)
       → 10010110
       → 128 + 16 + 4 + 2 = 150
```

## More Value Types

### Signed Integers

如上所述，与wire类型0关联的所有协议缓冲区类型都被编码为VARINTS。  但是，当涉及编码负数时，签名的int类型（sint32和sint64）和“标准”int类型（int32和int64）存在重要差异。  如果使用Int32或Int64作是负数的类型，则结果Varint始终为十个字节，这种编码方式是正确的但效率低下。 如果将负数全部映射为无符号整数，即所生成的Varint使用Zigzag编码，这样效率就更高效。

Zigzag编码映射到无符号整数的整数，以便具有小绝对值（例如，-1）的数字也具有小的Varint编码值。 它以通过正和负整数来回“Zig-Zags”的方式来实现这一点，使得-1被编码为1,1，编码为2，-2编码为3，等等，如您 可以在下表中看到：

| Signed Original | Encoded As |
| --------------- | ---------- |
| 0               | 0          |
| -1              | 1          |
| 1               | 2          |
| -2              | 3          |
| 2147483647      | 4294967294 |
| -2147483648     | 4294967295 |

In other words, each value `n` is encoded using：

```bash
(n << 1) ^ (n >> 31)
```

for `sint32`s, or

```bash
(n << 1) ^ (n >> 63)
```

for the 64-bit version.

注意，第二个移位(n >> 31)部分是算术移位。换句话说，移位的结果要么是一个全0位(如果n是正的)，要么是一个全1位(如果n是负的)。(^：位异或)

当sint32或sin64被解析时，它的值被解码回原来的有符号的版本。

### Non-varint Numbers

Non-varint numeric types are simple – `double` and `fixed64` have wire type 1, which tells the parser to expect a fixed 64-bit lump of data; similarly `float` and `fixed32` have wire type 5, which tells it to expect 32 bits. In both cases the values are stored in little-endian byte order.

### Strings

A wire type of 2 (length-delimited) means that the value is a varint  encoded length followed by the specified number of bytes of data.

```protobuf
message Test2 {
  optional string b = 2;
}
```

Setting the value of b to "testing" gives you:

```bash
12 07 [74 65 73 74 69 6e 67]
```

The bytes in [brackets] are the UTF8 of "testing". The key here is 0x12. It's parsed:

```bash
0x12
→ 0001 0010  (binary representation)
→ 00010 010  (regroup bits)
→ field_number = 2, wire_type = 2
```

The length varint in the value is 7 and lo and behold, we find seven bytes following it – our string.

## Embedded Messages

Here's a message definition with an embedded message of our example type, Test1:

```bash
message Test3 { optional Test1 c = 3;}
```

And here's the encoded version, again with the Test1's `a` field set to 150:

```bash
 1a 03 08 96 01
```

As you can see, the last three bytes are exactly the same as our first example (`08 96 01`), and they're preceded by the number 3 – embedded messages are treated in exactly the same way as strings (wire type = 2).

## Optional And Repeated Elements

如果proto2消息定义有重复的元素(没有[packed=true]选项)，则编码的消息有0个或多个具有相同字段号的键值对。这些重复的值不需要连续出现;它们可能与其他领域交织。解析时保留元素相对于其他字段的顺序，但是丢失了相对于其他字段的顺序。在proto3中，重复字段使用打包编码，你可以阅读下面的内容。

对于proto3中的任何非重复字段或proto2中的可选字段，编码的消息可能有也可能没有带该字段号的键值对。

Normally, an encoded message would never have more than one instance  of a non-repeated field. However, parsers are expected to handle the  case in which they do. For numeric types and strings, if the same field  appears multiple times, the parser accepts the *last* value it sees. For embedded message fields, the parser merges multiple instances of the same field, as if with the `Message::MergeFrom` method – that is, all singular scalar fields in the latter instance  replace those in the former, singular embedded messages are merged, and  repeated fields are concatenated. The effect of these rules is that  parsing the concatenation of two encoded messages produces exactly the  same result as if you had parsed the two messages separately and merged  the resulting objects. That is, this:

```c++
MyMessage message;message.ParseFromString(str1 + str2);
```

is equivalent to this:

```c++
MyMessage message, message2;message.ParseFromString(str1);message2.ParseFromString(str2);message.MergeFrom(message2);
```

This property is occasionally useful, as it allows you to merge two messages even if you do not know their types.

### Packed Repeated Fields

版本2.1.0引入了打包重复字段，在proto2中，它的声明类似于重复字段，但使用了特殊的[packed=true]选项。在proto3中，标量数值类型的重复字段被默认打包。这些函数类似重复字段，但编码方式不同。包含零元素的打包重复字段不会出现在编码的信息中。否则，该字段的所有元素将被打包成单个键-值对，wire类型为2(以长度分隔)。每个元素都以正常的方式编码，除了前面没有键。

For example, imagine you have the message type:

```protobuf
message Test4 {
  repeated int32 d = 4 [packed=true];
}
```

Now let's say you construct a `Test4`, providing the values 3, 270, and 86942 for the repeated field `d`. Then, the encoded form would be:

```c++
22        // key (field number 4, wire type 2)
06        // payload size (6 bytes)
03        // first element (varint 3)
8E 02     // second element (varint 270)
9E A7 05  // third element (varint 86942)
```

只有重复的原始数字类型（使用Varint，32位或64位线类型）的类型可以被声明为“Packed”。

请注意，尽管通常没有理由为一个已打包的重复字段编码多个键值对，但编码器必须准备好接受多个键值对。在这种情况下，有效载荷应该是串联的。每对元素必须包含一个整数。

协议缓冲区解析器必须能够解析被编译成压缩的重复字段，就像它们没有被压缩一样，反之亦然。这允许以向前和向后兼容的方式向现有字段添加[packed=true]。

## Field Order

在.proto文件中，字段编号可以以任何顺序使用。选择的顺序对消息序列化的方式没有影响。

序列化消息时，没有保证应该写入的已知或未知字段的保证顺序。 序列化顺序是一个实施细节，任何特定实现的细节可能会在将来发生变化。 因此，协议缓冲区解析器必须能够以任何顺序解析字段。

### Implications

- 不要假设序列化消息的字节输出是稳定的。 对于具有表示其他序列化协议缓冲区消息的传递字节字段的消息尤其如此。
- 默认情况下，在同一协议缓冲区消息实例上重复调用序列化方法可能不会返回相同的字节输出;也就是说，默认的序列化是不确定性的。
  - 确定性序列化仅保证特定二进制的相同字节输出。 字节输出可能会在二进制文件的不同版本上发生变化。

- The following checks may fail for a protocol buffer message instance `foo`.
  - `foo.SerializeAsString() == foo.SerializeAsString()`
  - `Hash(foo.SerializeAsString()) == Hash(foo.SerializeAsString())`
  - `CRC(foo.SerializeAsString()) == CRC(foo.SerializeAsString())`
  - `FingerPrint(foo.SerializeAsString()) == FingerPrint(foo.SerializeAsString())`

- 下面是几个示例场景，其中逻辑上等效的协议缓冲区消息foo和bar可能序列化为不同的字节输出。
  - `bar` is serialized by an old server that treats some fields as unknown.
  - `bar` is serialized by a server that is implemented in a different programming language and serializes fields in different order.
  - `bar` has a field that serializes in non-deterministic manner.
  - `bar` has a field that stores a serialized byte output of a protocol buffer message which is serialized differently.
  - `bar` is serialized by a new server that serializes fields in different order due to an implementation change.
  - Both `foo` and `bar` are concatenation of individual messages but with different order.

# Techniques

此页面描述了一些用于处理协议缓冲区的常用设计模式。 您还可以向协议缓冲区讨论组发送设计和使用问题。

## Streaming Multiple Messages

如果要将多个消息写入单个文件或流，请取决于跟踪一个消息结束的位置，下一个开始。 协议缓冲区线格式不是自定义，因此协议缓冲区解析器无法确定消息以自己结束的位置。 解决此问题的最简单方法是在写入消息本身之前编写每个消息的大小。 当您读回消息时，读取大小，然后将字节读入单独的缓冲区，然后从该缓冲区解析。   （如果要避免将字节复制到单独的缓冲区，请签出CodedInputStream类（在C ++和Java中）可以被告知将读取为一定数量的字节。）

## Large Data Sets

协议缓冲区不是设计来处理大消息的。根据一般经验，如果处理的消息大于1兆字节，则可能需要考虑另一种策略。

也就是说，协议缓冲区非常适合处理大数据集中的单个消息。  通常，大型数据集真的只是小块的集合，其中每个小件可以是结构化的数据。  尽管协议缓冲区立即无法处理整个集合，但使用协议缓冲区来对每件作品进行编码，请大大简化您的问题：现在您所需要的只是处理一组字节字符串而不是一组结构。

协议缓冲区不包括对大型数据集的任何内置支持，因为不同的情况需要不同的解决方案。有时一个简单的记录列表就可以了，而有时您可能想要一个更像数据库的东西。每个解决方案都应该开发为一个单独的库，这样只有那些需要它的人才需要为此付出代价。

## Self-describing Messages

协议缓冲区不包含它们自己类型的描述。因此，如果只给出一个原始消息，而没有相应的.proto文件定义其类型，则很难提取任何有用的数据。

 但是，请注意.proto文件的内容本身可以使用协议缓冲区来表示。源代码包中的src/google/protobuf/descriptor.proto文件定义了所涉及的消息类型。protoc可以使用--descriptor_set_out选项输出FileDescriptorSet，它表示一组.proto文件。这样，您就可以定义一个self-describing的协议消息，如下所示： 

```protobuf
syntax = "proto3";

import "google/protobuf/any.proto";
import "google/protobuf/descriptor.proto";

message SelfDescribingMessage {
  // Set of FileDescriptorProtos which describe the type and its dependencies.
  google.protobuf.FileDescriptorSet descriptor_set = 1;

  // The message and its type, encoded as an Any message.
  google.protobuf.Any message = 2;
}
```

通过使用DynamicMessage等类（可用于C ++和Java），然后可以编写可以操作selfdescribingmessages的工具。

综上所述，协议缓冲区库中没有包含此功能的原因是我们在谷歌中从未使用过它。

这种技术需要使用描述符支持动态消息。在使用self-describing信息之前，请检查您的平台是否支持此功能。

# Protocol Buffer Basics: C++

本教程提供了基本的C ++程序员与协议缓冲区一起使用的介绍。 通过步行创建一个简单的示例应用程序，它向您展示如何:

- Define message formats in a `.proto` file.  
- Use the protocol buffer compiler.  
- Use the C++ protocol buffer API to write and read messages.

This isn't a comprehensive guide to using protocol buffers in C++.  For more detailed reference information, see the [Protocol Buffer Language Guide](https://developers.google.com/protocol-buffers/docs/proto), the [C++ API Reference](https://developers.google.com/protocol-buffers/docs/reference/cpp), the [C++ Generated Code Guide](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated), and the [Encoding Reference](https://developers.google.com/protocol-buffers/docs/encoding).

## Why Use Protocol Buffers?

我们将要使用的示例是一个非常简单的“地址簿”应用程序，它可以从文件中读取和写入人们的联系方式。地址簿中的每个人都有一个姓名、ID、电子邮件地址和联系电话号码。

-  原始内存数据结构可以二进制形式发送/保存。随着时间的推移，这是一种脆弱的方法，因为接收/读取代码必须使用完全相同的内存布局、endianness等进行编译。此外，由于文件以原始格式积累数据，并且为该格式连接的软件副本分布在各地，因此很难扩展该格式。 
- 您可以将ad-hoc方法编码为将数据项编码为单个字符串 - 例如编码4个ints为“12：3：-23：67”。 这是一种简单灵活的方法，但它需要写入一次性编码和解析代码，并且解析强加了小的运行时间成本。 这最适合编码非常简单的数据。
-  将数据序列化为XML。这种方法非常有吸引力，因为XML是（某种程度上）人类可读的，并且有许多语言的绑定库。如果您想与其他应用程序/项目共享数据，这是一个不错的选择。然而，XML是众所周知的空间密集型，编码/解码它会给应用程序带来巨大的性能损失。此外，在XML DOM树中导航比在类中导航简单字段要复杂得多。 

协议缓冲区是灵活，高效，自动化的解决方案，以解决此问题。  使用协议缓冲区，您可以编写.proto您希望存储的数据结构的描述。  根据，协议缓冲区编译器创建一个类，它用有效的二进制格式实现自动编码和解析协议缓冲区数据。  生成的类为构成协议缓冲区的字段提供了getter和setter，并处理读取和将协议缓冲区的详细信息作为一个单元。  重要的是，协议缓冲器格式支持随时间扩展格式的想法，使得代码仍然可以读取以旧格式编码的数据。

## Where to Find the Example Code

The example code is included in the source code package, under the "examples" directory.  [Download it here.](https://developers.google.com/protocol-buffers/docs/downloads)

## Defining Your Protocol Format

要创建您的地址簿应用程序，您需要从.proto文件开始。  .proto文件中的定义很简单：您为要序列化的每个数据结构添加消息，然后为消息中的每个字段指定名称和类型。 这是.proto文件，用于定义您的消息，`addressbook.proto`.

```protobuf
syntax = "proto2";

package tutorial;

message Person {
  optional string name = 1;
  optional int32 id = 2;
  optional string email = 3;

  enum PhoneType {
    MOBILE = 0;
    HOME = 1;
    WORK = 2;
  }

  message PhoneNumber {
    optional string number = 1;
    optional PhoneType type = 2 [default = HOME];
  }

  repeated PhoneNumber phones = 4;
}

message AddressBook {
  repeated Person people = 1;
}
```

如您所见，语法类似于C ++或Java。 让我们浏览文件的每个部分，看看它的内容。

.proto文件从包声明开始，有助于防止不同项目之间的命名冲突。 在C ++中，您生成的类将放在匹配包名称的命名空间中。

Next, you have your message definitions.  A message is just an  aggregate containing a set of typed fields.  Many standard simple data  types are available as field types, including `bool`, `int32`, `float`, `double`, and `string`. You can also add further structure to your messages by using other message types as field types – in the above example the `Person` message contains `PhoneNumber` messages, while the `AddressBook` message contains `Person` messages.  You can even define message types nested inside other messages – as you can see, the `PhoneNumber` type is defined inside `Person`.  You can also define `enum` types if you want one of your fields to have one of a predefined list  of values – here you want to specify that a phone number can be one of `MOBILE`, `HOME`, or `WORK`.

The " = 1", " = 2" markers on each element identify the unique  "tag" that field uses in the binary encoding. Tag numbers 1-15 require  one less byte to encode than higher numbers, so as an optimization you  can decide to use those tags for the commonly used or repeated elements, leaving tags 16 and higher for less-commonly used optional elements.   Each element in a repeated field requires re-encoding the tag number, so repeated fields are particularly good candidates for this optimization.

每个字段必须用以下一个修改器之一注释：

- `optional`: 该字段可以设置，也可以不设置。如果没有设置可选字段值，则使用默认值。对于简单类型，您可以指定自己的默认值，就像我们在示例中为电话号码类型所做的那样。否则，使用系统默认值:0表示数字类型，空字符串表示字符串，false表示bool值。对于嵌入的消息，默认值总是消息的“默认实例”或“原型”，它没有设置任何字段。调用访问器获取未显式设置的可选(或必选)字段的值总是返回该字段的默认值 .

- `repeated`:该字段可以重复任何次数(包括零)。重复值的顺序将在协议缓冲区中保留。可以将重复字段看作动态大小的数组。 

- `required`: 必须为字段提供一个值，否则该消息将被认为是“未初始化的”。如果libprotobuf以调试模式编译，序列化未初始化的消息将导致断言失败。在优化的构建中，检查将被跳过，消息将被写入。但是，解析未初始化的消息总是会失败(通过从parse方法返回false)。除此之外，必选字段的行为与可选字段完全相同。

  注： 你应该非常小心地按照要求标记字段。如果在某个时候，你希望停止写或发送一个必填字段，那么将字段转换成一个可选字段将是个问题。老读者会考虑没有这个字段的消息是不完整的，并且可能会无意地拒绝或丢弃它们。您应该考虑为缓冲区编写特定于应用程序的自定义验证例程。在谷歌内部，必填字段非常不受欢迎；在proto2语法中定义的大多数消息都使用可选的和重复的(Proto3根本不支持必填字段。） 

You'll find a complete guide to writing `.proto` files – including all the possible field types –  in the [Protocol Buffer Language Guide](https://developers.google.com/protocol-buffers/docs/proto). Don't go looking for facilities similar to class inheritance, though – protocol buffers don't do that.

## Compiling Your Protocol Buffers

现在已经有了.proto，接下来需要做的事情是生成需要读取和写入AddressBook(以及Person和PhoneNumber)消息的类。为此，您需要在.proto上运行协议缓冲编译器协议：

- If you haven't installed the compiler, [download the package](https://developers.google.com/protocol-buffers/docs/downloads) and follow the instructions in the README.  

- Now run the compiler, specifying the source directory (where  your application's source code lives – the current directory is used if  you don't provide a value), the destination directory (where you want  the generated code to go; often the same as `$SRC_DIR`), and the path to your `.proto`.  In this case, you...:

```protobuf
protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/addressbook.proto
```

1. Because you want C++ classes, you use the `--cpp_out` option – similar options are provided for other supported languages.

This generates the following files in your specified destination directory:  

- `addressbook.pb.h`, the header which declares your generated classes.    
- `addressbook.pb.cc`, which contains the implementation of your classes.  

## The Protocol Buffer API

让我们看看生成的一些代码，看看编译器为您创建了哪些类和函数。如果您查看addressbook.pb.h，您可以看到您在addressbook.proto中指定的每个消息都有一个类。仔细查看Person类，您可以看到编译器为每个字段生成了访问器。例如，对于名称、id、电子邮件和电话字段，您可以使用这些方法

```protobuf
  // name
  inline bool has_name() const;
  inline void clear_name();
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline ::std::string* mutable_name();

  // id
  inline bool has_id() const;
  inline void clear_id();
  inline int32_t id() const;
  inline void set_id(int32_t value);

  // email
  inline bool has_email() const;
  inline void clear_email();
  inline const ::std::string& email() const;
  inline void set_email(const ::std::string& value);
  inline void set_email(const char* value);
  inline ::std::string* mutable_email();

  // phones
  inline int phones_size() const;
  inline void clear_phones();
  inline const ::google::protobuf::RepeatedPtrField< ::tutorial::Person_PhoneNumber >& phones() const;
  inline ::google::protobuf::RepeatedPtrField< ::tutorial::Person_PhoneNumber >* mutable_phones();
  inline const ::tutorial::Person_PhoneNumber& phones(int index) const;
  inline ::tutorial::Person_PhoneNumber* mutable_phones(int index);
  inline ::tutorial::Person_PhoneNumber* add_phones();
```

As you can see, the getters have exactly the name as the field in lowercase, and the setter methods begin with `set_`. There are also `has_` methods for each singular (required or optional) field which return true if that field has been set. Finally, each field has a `clear_` method that un-sets the field back to its empty state.

While the numeric `id` field just has the basic accessor set described above, the `name` and `email` fields have a couple of extra methods because they're strings – a `mutable_` getter that lets you get a direct pointer to the string, and an extra setter.  Note that you can call `mutable_email()` even if `email` is not already set; it will be initialized to an empty string  automatically.  If you had a singular message field in this example, it  would also have a `mutable_` method but not a `set_` method.

Repeated fields also have some special methods – if you look at the methods for the repeated `phones` field, you'll see that you can

- check the repeated field's `_size` (in other words, how many phone numbers are associated with this `Person`).
- get a specified phone number using its index.
- update an existing phone number at the specified index.
- add another phone number to the message which you can then edit (repeated scalar types have an `add_` that just lets you pass in the new value).

For more information on exactly what members the protocol compiler generates for any particular field definition, see the [C++ generated code reference](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated).

### Enums and Nested Classes

The generated code includes a `PhoneType` enum that corresponds to your `.proto` enum.  You can refer to this type as `Person::PhoneType` and its values as `Person::MOBILE`, `Person::HOME`, and `Person::WORK` (the implementation details are a little more complicated, but you don't need to understand them to use the enum).

The compiler has also generated a nested class for you called `Person::PhoneNumber`.  If you look at the code, you can see that the "real" class is actually called `Person_PhoneNumber`, but a typedef defined inside `Person` allows you to treat it as if it were a nested class.  The only case  where this makes a difference is if you want to forward-declare the  class in another file – you cannot forward-declare nested types in C++,  but you can forward-declare `Person_PhoneNumber`.

### Standard Message Methods

Each message  class also contains a number of other methods that let you check or manipulate the entire message, including:

- `bool IsInitialized() const;`: checks if all the required fields have been set.  
- `string DebugString() const;`: returns a human-readable representation of the message, particularly useful for debugging.  
- `void CopyFrom(const Person& from);`: overwrites the message with the given message's values.  
- `void Clear();`: clears all the elements back to the empty state.

These and the I/O methods described in the following section implement the `Message` interface shared by all C++ protocol buffer classes. For more info, see the [complete API documentation for `Message`](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.message#Message).

### Parsing and Serialization

Finally, each protocol buffer class has methods for writing and reading messages of your chosen type using the protocol buffer [binary format](https://developers.google.com/protocol-buffers/docs/encoding).  These include:

- `bool SerializeToString(string* output) const;`: serializes the message and stores the bytes in the given string.  Note that the bytes are binary, not text; we only use the `string` class as a convenient container.  
- `bool ParseFromString(const string& data);`: parses a message from the given string.  
- `bool SerializeToOstream(ostream* output) const;`: writes the message to the given C++ `ostream`.  
- `bool ParseFromIstream(istream* input);`: parses a message from the given C++ `istream`.

These are just a couple of the options provided for parsing and serialization.  Again, see the [`Message` API reference](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.message#Message) for a complete list.

注：**Protocol Buffers and Object Oriented Design** Protocol buffer classes are basically dumb data holders (like structs in C);  they don't make good first class citizens in an object model.  If you want to add richer behavior to a generated class,  the best way to do this is to wrap the generated protocol buffer class in an application-specific class.  Wrapping protocol buffers is also a good idea if you don't have control over the design of the `.proto` file (if, say, you're reusing one from another project). In that case,  you can use the wrapper class to craft an interface better suited to the unique environment of your application: hiding some data and methods,  exposing convenience functions, etc. **You should never add behaviour to the generated classes by inheriting from them**. This will break internal mechanisms and is not good object-oriented practice anyway.

## Writing A Message

现在让我们尝试使用协议缓冲区类。 您希望您的地址簿应用程序能够做的第一件事就是将个人详细信息写入您的地址簿文件。 为此，您需要创建和填充协议缓冲区类的实例，然后将它们写入输出流。

下面是一个程序，它从文件中读取AddressBook，根据用户输入向其添加一个新的Person，并将新的AddressBook再次写入文件。突出显示了直接调用或引用协议编译器生成的代码的部分。

```c++
#include <iostream>
#include <fstream>
#include <string>
#include "addressbook.pb.h"
using namespace std;

// This function fills in a Person message based on user input.
void PromptForAddress(tutorial::Person* person) {
  cout << "Enter person ID number: ";
  int id;
  cin >> id;
  person->set_id(id);
  cin.ignore(256, '\n');

  cout << "Enter name: ";
  getline(cin, *person->mutable_name());

  cout << "Enter email address (blank for none): ";
  string email;
  getline(cin, email);
  if (!email.empty()) {
    person->set_email(email);
  }

  while (true) {
    cout << "Enter a phone number (or leave blank to finish): ";
    string number;
    getline(cin, number);
    if (number.empty()) {
      break;
    }

    tutorial::Person::PhoneNumber* phone_number = person->add_phones();
    phone_number->set_number(number);

    cout << "Is this a mobile, home, or work phone? ";
    string type;
    getline(cin, type);
    if (type == "mobile") {
      phone_number->set_type(tutorial::Person::MOBILE);
    } else if (type == "home") {
      phone_number->set_type(tutorial::Person::HOME);
    } else if (type == "work") {
      phone_number->set_type(tutorial::Person::WORK);
    } else {
      cout << "Unknown phone type.  Using default." << endl;
    }
  }
}

// Main function:  Reads the entire address book from a file,
//   adds one person based on user input, then writes it back out to the same
//   file.
int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
    return -1;
  }

  tutorial::AddressBook address_book;

  {
    // Read the existing address book.
    fstream input(argv[1], ios::in | ios::binary);
    if (!input) {
      cout << argv[1] << ": File not found.  Creating a new file." << endl;
    } else if (!address_book.ParseFromIstream(&input)) {
      cerr << "Failed to parse address book." << endl;
      return -1;
    }
  }

  // Add an address.
  PromptForAddress(address_book.add_people());

  {
    // Write the new address book back to disk.
    fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    if (!address_book.SerializeToOstream(&output)) {
      cerr << "Failed to write address book." << endl;
      return -1;
    }
  }

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
```

注意Google_protobuf_verify_version宏。  这是一个很好的做法 - 虽然没有严格必要 - 在使用C ++协议缓冲区库之前执行此宏。  它验证了您没有意外链接到库的版本，这与您编译的标题版本不兼容。 如果检测到版本不匹配，则程序将中止。  请注意，每一个.pb.cc文件都会自动调用启动时的此宏。

还要注意程序结束时对ShutdownProtobufLibrary()的调用。所有这些都是删除协议缓冲区库分配的所有全局对象。这对大多数程序来说都是不必要的，因为进程无论如何都会退出，而操作系统会负责回收它的所有内存。但是，如果您使用内存泄漏检查器，要求释放每一个最后的对象，或者如果您正在编写一个可能被单个进程加载和卸载多次的库，那么您可能希望强制协议缓冲区清除所有内容。

## Reading A Message

当然，如果你不能从中获得任何信息，通讯录也没有多大用处!这个示例读取由上面的示例创建的文件，并打印其中的所有信息。

```c++
#include <iostream>
#include <fstream>
#include <string>
#include "addressbook.pb.h"
using namespace std;

// Iterates though all people in the AddressBook and prints info about them.
void ListPeople(const tutorial::AddressBook& address_book) {
  for (int i = 0; i < address_book.people_size(); i++) {
    const tutorial::Person& person = address_book.people(i);

    cout << "Person ID: " << person.id() << endl;
    cout << "  Name: " << person.name() << endl;
    if (person.has_email()) {
      cout << "  E-mail address: " << person.email() << endl;
    }

    for (int j = 0; j < person.phones_size(); j++) {
      const tutorial::Person::PhoneNumber& phone_number = person.phones(j);

      switch (phone_number.type()) {
        case tutorial::Person::MOBILE:
          cout << "  Mobile phone #: ";
          break;
        case tutorial::Person::HOME:
          cout << "  Home phone #: ";
          break;
        case tutorial::Person::WORK:
          cout << "  Work phone #: ";
          break;
      }
      cout << phone_number.number() << endl;
    }
  }
}

// Main function:  Reads the entire address book from a file and prints all
//   the information inside.
int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
    return -1;
  }

  tutorial::AddressBook address_book;

  {
    // Read the existing address book.
    fstream input(argv[1], ios::in | ios::binary);
    if (!address_book.ParseFromIstream(&input)) {
      cerr << "Failed to parse address book." << endl;
      return -1;
    }
  }

  ListPeople(address_book);

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
```

## Extending a Protocol Buffer

在您发布使用协议缓冲区的代码后，您肯定会想要“改进”协议缓冲区的定义。如果你想要你的新缓冲区向后兼容，你的旧缓冲区向前兼容，你几乎肯定想要这样，那么有一些规则你需要遵守。在协议缓冲区的新版本中

- you *must not* change the tag numbers of any existing fields.
- you *must not* add or delete any required fields.
- you *may* delete optional or repeated fields.
- you *may* add new optional or repeated fields but you must use fresh tag numbers  (i.e. tag numbers that were never used in this protocol buffer, not even by deleted fields).

(There are [some exceptions](https://developers.google.com/protocol-buffers/docs/proto#updating) to these rules, but they are rarely used.)

If you follow these rules, old code will happily read new messages and  simply ignore any new fields. To the old code, optional fields that were deleted will simply have their default value, and deleted repeated  fields will be empty. New code will also transparently read old  messages. However, keep in mind that new optional fields will not be  present in old messages, so you will need to either check explicitly  whether they're set with `has_`, or provide a reasonable default value in your `.proto` file with `[default = value]` after the tag number. If the default value is not specified for an  optional element, a type-specific default value is used instead: for  strings, the default value is the empty string. For booleans, the  default value is false. For numeric types, the default value is zero.  Note also that if you added a new repeated field, your new code will not be able to tell whether it was left empty (by new code) or never set at all (by old code) since there is no `has_` flag for it.

## Optimization Tips

The C++ Protocol Buffers library is extremely heavily optimized.  However,  proper usage can improve performance even more.  Here are some tips for  squeezing every last drop of speed out of the library:

- Reuse message objects when possible.  Messages try to keep around  any memory they allocate for reuse, even when they are cleared.  Thus,  if you are handling many messages with the same type and similar  structure in succession, it is a good idea to reuse the same message  object each time to take load off the memory allocator.  However,  objects can become bloated over time, especially if your messages vary  in "shape" or if you occasionally construct a message that is much  larger than usual.  You should monitor the sizes of your message objects by calling the `SpaceUsed` method and delete them once they get too big.  
- Your system's memory allocator may not be well-optimized for allocating lots of small objects from multiple threads.  Try using [Google's tcmalloc](https://github.com/gperftools/gperftools) instead.

## Advanced Usage

Protocol buffers have uses that go beyond simple accessors and serialization.  Be sure to explore the [C++ API reference](https://developers.google.com/protocol-buffers/docs/reference/cpp) to see what else you can do with them.

One key feature provided by protocol message classes is *reflection*.  You can iterate over the fields of a message and manipulate their  values without writing your code against any specific message type.  One very useful way to use reflection is for converting protocol messages  to and from other encodings, such as XML or JSON.  A more advanced use  of reflection might be to find differences between two messages of the  same type, or to develop a sort of "regular expressions for protocol  messages" in which you can write expressions that match certain message  contents.  If you use your imagination, it's possible to apply Protocol  Buffers to a much wider range of problems than you might initially  expect!

​      Reflection is provided by the [`Message::Reflection` interface](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.message#Message.Reflection).                  

# C++ Generated Code

此页面确切地描述了协议缓冲区编译器为任何给定协议定义生成的C ++代码。  突出显示PROTO2和PROTO3生成的代码之间的任何差异 -  请注意，这些差异在本文档中描述的生成代码中，而不是两个版本中相同的基本消息类/接口。  在阅读本文档之前，您应该阅读Proto2语言指南和/或Proto3语言指南。

## Compiler Invocation

协议缓冲区编译器在使用--cpp_out =命令行标志调用时生成C ++输出。  the -cpp_out =选项的参数是您希望编译器编写C  ++输出的目录。 编译器为每个.proto文件生存一个.h件和一个.cc文件。  通过.proto文件的名称计算输出文件的名称：

- The extension (`.proto`) is replaced with either `.pb.h` or `.pb.cc` for the header or implementation file, respectively.
- The proto path (specified with the `--proto_path=` or `-I` command-line flag) is replaced with the output path (specified with the `--cpp_out=` flag).

So, for example, let's say you invoke the compiler as follows:

```bash
protoc --proto_path=src --cpp_out=build/gen src/foo.proto src/bar/baz.proto
```

The compiler will read the files `src/foo.proto` and `src/bar/baz.proto` and produce four output files:  `build/gen/foo.pb.h`, `build/gen/foo.pb.cc`, `build/gen/bar/baz.pb.h`, `build/gen/bar/baz.pb.cc`.  The compiler will automatically create the directory `build/gen/bar` if necessary, but it will *not* create `build` or `build/gen`; they must already exist.

## Packages

如果.proto文件包含包声明，则该文件的全部内容将被放置在相应的c++命名空间中。例如，给定包声明

```protobuf
package foo.bar;
```

文件中的所有声明都将驻留在foo::bar命名空间中。

## Messages

```protobuf
message Foo {}
```

协议缓冲编译器生成一个名为Foo的类，它public于 `google::protobuf::Message`. 类是一个具体类;没有任何纯虚方法是不被实现的。在Message中是虚的而不是纯虚的方法可能被Foo覆盖，也可能不被Foo覆盖，这取决于优化模式。默认情况下，Foo实现了所有方法的特殊版本以获得最大速度。但是，如果.proto文件包含这一行

```protobuf
option optimize_for = CODE_SIZE;
```

然后Foo将只覆盖函数所需的最小方法集，并依赖于其余的基于反射的实现。这大大减少了生成代码的大小，但也降低了性能。或者，如果.proto文件包含:

```protobuf
option optimize_for = LITE_RUNTIME;
```

resource-constrained systems such as mobile phones.

然后Foo会包含所有方法的快速实现，但是会实现谷歌::protobuf::MessageLite接口，它只包含Message方法的子集。特别是，它不支持描述符或反射。但是，在这种模式下，生成的代码只需要链接到libprotobuf-lite。(libprotobuf-lite。)而不是libprotobuf。(libprotobuf.lib)。“lite”库比完整库小得多，更适合于资源受限的系统。

你不应该创建你自己的Foo子类。如果您继承这个类并覆盖一个虚拟方法，则覆盖可能会被忽略，因为许多生成的方法调用都是去虚拟化的，以提高性能。

Message接口定义了一些方法，这些方法允许您检查、操作、读取或写入整个消息，包括解析二进制字符串和将其序列化为二进制字符串。

- `bool ParseFromString(const string& data)`: Parse the message from the given serialized binary string (also known as wire format).  
- `bool SerializeToString(string* output) const`: Serialize the given message to a binary string.  
- `string DebugString()`: Return a string giving the `text_format` representation of the proto (should only be used for debugging).

除了这些方法外，Foo类还定义以下方法：

- `Foo()`: Default constructor.  
- `~Foo()`: Default destructor.  
- `Foo(const Foo& other)`: Copy constructor.  
- `Foo& operator=(const Foo& other)`: Assignment operator.  
- `void Swap(Foo* other)`: Swap content with another message.  
- `const UnknownFieldSet& unknown_fields() const`: Returns the set of unknown fields encountered while parsing this message.  
- `UnknownFieldSet* mutable_unknown_fields()`: Returns a pointer to the mutable set of unknown fields encountered while parsing this message.

该类还定义了以下静态方法：

- `static const Descriptor* descriptor()`: Returns the type's descriptor. This contains information about the  type, including what fields it has and what their types are.  This can  be used with [reflection](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.message#Message.Reflection) to inspect fields programmatically.  
- `static const Foo& default_instance()`: Returns a const singleton instance of `Foo` which is identical to a newly-constructed instance of `Foo` (so all singular fields are unset and all repeated fields are empty).   Note that the default instance of a message can be used as a factory by  calling its `New()` method.

### Nested Types

A message can be declared inside another message.  For example: ` message Foo {  message Bar {  } } `

在这种情况下，编译器生成两个类：foo和foo_bar。 此外，编译器在foo内生成一个类型：

```c++
typedef Foo_Bar Bar;
```

这意味着您可以使用嵌套类型的类，就像它是嵌套类foo :: bar。 但请注意，C ++不允许嵌套类型进行前向声明。 如果要在另一个文件中前向声明并使用该声明，则必须将其标识为Foo_Bar。

## Fields

除了前一节中描述的方法外，协议缓冲区编译器为.proto文件中消息中定义的每个字段生成一组访问器方法。

和访问器方法一样，编译器为每个包含其字段号的字段生成一个整型常量。常量名是字母k，后面是转换为驼峰大小写的字段名，然后是FieldNumber。例如，给定optional int32 foo_bar = 5;，编译器将生成 `static const int kFooBarFieldNumber = 5;`.

对于返回const引用的字段访问器，当对消息进行下一次修改访问时，该引用可能会失效。这包括调用任何字段的任何非const访问器，调用从Message继承的任何非const方法，或通过其他方式修改消息(例如，使用消息作为Swap()的参数)。相应地，只有在没有同时对消息进行修改访问的情况下，才保证在访问器的不同调用之间返回的引用的地址相同。

对于返回指针的字段访问器，当对消息进行下一次修改或非修改访问时，该指针可能会失效。这包括，无论常量如何，调用任何字段的任何访问器，调用从Message继承的任何方法，或通过其他方式访问消息(例如，使用复制构造函数复制消息)。相应地，也不能保证在访问器的两个不同调用之间返回的指针的值是相同的。

### Singular Numeric Fields (proto2)

For either of these field definitions:

```protobuf
optional int32 foo = 1;
required int32 foo = 1;
```

编译器将生成以下访问者方法：

- `bool has_foo() const`: Returns `true` if the field is set.  
- `int32 foo() const`: Returns the current value of the field.  If the field is not set, returns the default value.  
- `void set_foo(int32 value)`: Sets the value of the field.  After calling this, `has_foo()` will return `true` and `foo()` will return `value`.  
- `void clear_foo()`: Clears the value of the field.  After calling this, `has_foo()` will return `false` and `foo()` will return the default value.

For other numeric field types (including `bool`), `int32` is replaced with the corresponding C++ type according to the [scalar value types table](https://developers.google.com/protocol-buffers/docs/proto#scalar).

### Singular Numeric Fields (proto3)

For this field definition:

```
int32 foo = 1; 
```

The compiler will generate the following accessor methods:

- `int32 foo() const`: Returns the current value of the field.  If the field is not set, returns 0.  
- `void set_foo(int32 value)`: Sets the value of the field.  After calling this, `foo()` will return `value`.  
- `void clear_foo()`: Clears the value of the field.  After calling this, `foo()` will return 0.

For other numeric field types (including `bool`), `int32` is replaced with the corresponding C++ type according to the [scalar value types table](https://developers.google.com/protocol-buffers/docs/proto3#scalar).

### Singular String Fields (proto2)

For any of these field definitions:

```protobuf
optional string foo = 1;
required string foo = 1;
optional bytes foo = 1;
required bytes foo = 1;
```

The compiler will generate the following accessor methods:

- `bool has_foo() const`: Returns `true` if the field is set.  
- `const string& foo() const`: Returns the current value of the field.  If the field is not set, returns the default value.  
- `void set_foo(const string& value)`: Sets the value of the field.  After calling this, `has_foo()` will return `true` and `foo()` will return a copy of `value`.  
- `void set_foo(string&& value)` (C++11 and beyond): Sets the value of the field, moving from the passed string. After calling this, `has_foo()` will return `true` and `foo()` will return a copy of `value`.  
- `void set_foo(const char* value)`: Sets the value of the field using a C-style null-terminated string.  After calling this, `has_foo()` will return `true` and `foo()` will return a copy of `value`.  
- `void set_foo(const char* value, int size)`: Like above, but the string size is given explicitly rather than determined by looking for a null-terminator byte.  
- `string* mutable_foo()`: Returns a pointer to the mutable `string` object that stores the field's value.  If the field was not set prior to the call, then the returned string will be empty (*not* the default value).  After calling this, `has_foo()` will return `true` and `foo()` will return whatever value is written into the given string.  
- `void clear_foo()`: Clears the value of the field.  After calling this, `has_foo()` will return `false` and `foo()` will return the default value.    
- `void set_allocated_foo(string* value)`: Sets the `string` object to the field and frees the previous field value if it exists. If the `string` pointer is not `NULL`, the message takes ownership of the allocated `string` object and `has_foo()` will return `true`.  The message is free to delete the allocated `string` object at any time, so references to the object may be invalidated. Otherwise, if the `value` is `NULL`, the behavior is the same as calling `clear_foo()`.  
- `string* release_foo()`: Releases the ownership of the field and returns the pointer of the `string` object. After calling this, caller takes the ownership of the allocated `string` object, `has_foo()` will return `false`, and `foo()` will return the default value.



### Singular String Fields (proto3)

For any of these field definitions:

```protobuf
string foo = 1; 
bytes foo = 1; 
```

The compiler will generate the following accessor methods:

- `const string& foo() const`: Returns the current value of the field.  If the field is not set, returns the empty string/empty bytes.  
- `void set_foo(const string& value)`: Sets the value of the field.  After calling this, `foo()` will return a copy of `value`.  
- `void set_foo(string&& value)` (C++11 and beyond): Sets the value of the field, moving from the passed string. After calling this, `foo()` will return a copy of `value`.  
- `void set_foo(const char* value)`: Sets the value of the field using a C-style null-terminated string.  After calling this, `foo()` will return a copy of `value`.  
- `void set_foo(const char* value, int size)`: Like above, but the string size is given explicitly rather than determined by looking for a null-terminator byte.  
- `string* mutable_foo()`: Returns a pointer to the mutable `string` object that stores the field's value.  If the field was not set prior  to the call, then the returned string will be empty.  After calling  this, `foo()` will return whatever value is written into the given string.  
- `void clear_foo()`: Clears the value of the field.  After calling this, `foo()` will return the empty string/empty bytes.  
- `void set_allocated_foo(string* value)`: Sets the `string` object to the field and frees the previous field value if it exists. If the `string` pointer is not `NULL`, the message takes ownership of the allocated `string` object. The message is free to delete the allocated `string` object at any time, so references to the object may be invalidated. Otherwise, if the `value` is `NULL`, the behavior is the same as calling `clear_foo()`.  
- `string* release_foo()`: Releases the ownership of the field and returns the pointer of the `string` object. After calling this, caller takes the ownership of the allocated `string` object and `foo()` will return the empty string/empty bytes.

### Singular Enum Fields (proto2)

Given the enum type:

```protobuf
enum Bar {
  BAR_VALUE = 0;
  OTHER_VALUE = 1;
}
```

For either of these field definitions:

```protobuf
optional Bar foo = 1;
required Bar foo = 1;
```

The compiler will generate the following accessor methods:

- `bool has_foo() const`: Returns `true` if the field is set.  
- `Bar foo() const`: Returns the current value of the field.  If the field is not set, returns the default value.  
- `void set_foo(Bar value)`: Sets the value of the field.  After calling this, `has_foo()` will return `true` and `foo()` will return `value`.  In debug mode (i.e. NDEBUG is not defined), if `value` does not match any of the values defined for `Bar`, this method will abort the process.  
- `void clear_foo()`: Clears the value of the field.  After calling this, `has_foo()` will return `false` and `foo()` will return the default value.

### Singular Enum Fields (proto3)

Given the enum type:

```protobuf
enum Bar {
  BAR_VALUE = 0;
  OTHER_VALUE = 1;
}
```

For this field definitions:

```protobuf
Bar foo = 1; 
```

The compiler will generate the following accessor methods:

- `Bar foo() const`: Returns the current value of the field.  If the field is not set, returns the default value (0).  
- `void set_foo(Bar value)`: Sets the value of the field.  After calling this, `foo()` will return `value`.  
- `void clear_foo()`: Clears the value of the field.  After calling this, `foo()` will return the default value.

### Singular Embedded Message Fields

Given the message type:

```protobuf
message Bar {} 
```

For any of these field definitions:

```protobuf
//proto2
optional Bar foo = 1;
required Bar foo = 1;
```

```protobuf
//proto3
Bar foo = 1;
```

The compiler will generate the following accessor methods:

- `bool has_foo() const`: Returns `true` if the field is set.  
- `const Bar& foo() const`: Returns the current value of the field.  If the field is not set, returns a `Bar` with none of its fields set (possibly `Bar::default_instance()`).  
- `Bar* mutable_foo()`: Returns a pointer to the mutable `Bar` object that stores the field's value.  If the field was not set prior to the call, then the returned `Bar` will have none of its fields set (i.e. it will be identical to a newly-allocated `Bar`).  After calling this, `has_foo()` will return `true` and `foo()` will return a reference to the same instance of `Bar`.  
- `void clear_foo()`: Clears the value of the field.  After calling this, `has_foo()` will return `false` and `foo()` will return the default value.  
- `void set_allocated_foo(Bar* bar)`: Sets the `Bar` object to the field and frees the previous field value if it exists. If the `Bar` pointer is not `NULL`, the message takes ownership of the allocated `Bar` object and `has_foo()` will return `true`. Otherwise, if the `Bar` is `NULL`, the behavior is the same as calling `clear_foo()`.  
- `Bar* release_foo()`: Releases the ownership of the field and returns the pointer of the `Bar` object. After calling this, caller takes the ownership of the allocated `Bar` object, `has_foo()` will return `false`, and `foo()` will return the default value.

### Repeated Numeric Fields

For this field definition:

```
repeated int32 foo = 1; 
```

The compiler will generate the following accessor methods:

- `int foo_size() const`: Returns the number of elements currently in the field.  
- `int32 foo(int index) const`: Returns the element at the given zero-based index. Calling this method  with index outside of [0, foo_size()) yields undefined behavior.  
- `void set_foo(int index, int32 value)`: Sets the value of the element at the given zero-based index.  
- `void add_foo(int32 value)`: Appends a new element to the end of the field with the given value.  
- `void clear_foo()`: Removes all elements from the field.  After calling this, `foo_size()` will return zero.  
- `const RepeatedField<int32>& foo() const`:  Returns the underlying `RepeatedField` that stores the field's elements.  This container class provides STL-like iterators and other methods.  
- `RepeatedField<int32>* mutable_foo()`:  Returns a pointer to the underlying mutable `RepeatedField` that stores the field's elements.  This container class provides STL-like iterators and other methods.

For other numeric field types (including `bool`), `int32` is replaced with the corresponding C++ type according to the [scalar value types table](https://developers.google.com/protocol-buffers/docs/proto#scalar).

### Repeated String Fields

For either of these field definitions:

```protobuf
repeated string foo = 1; 
repeated bytes foo = 1; 
```

The compiler will generate the following accessor methods:

- `int foo_size() const`: Returns the number of elements currently in the field.  
- `const string& foo(int index) const`: Returns the element at the given zero-based index. Calling this method  with index outside of [0, foo_size()) yields undefined behavior.  
- `void set_foo(int index, const string& value)`: Sets the value of the element at the given zero-based index.  
- `void set_foo(int index, const char* value)`: Sets the value of the element at the given zero-based index using a C-style null-terminated string.  
- `void set_foo(int index, const char* value, int size)`: Like above, but the string size is given explicitly rather than determined by looking for a null-terminator byte.  
- `string* mutable_foo(int index)`: Returns a pointer to the mutable `string` object that stores the value of the element at the given zero-based  index.  Calling this method with index outside of [0, foo_size()) yields undefined behavior.  
- `void add_foo(const string& value)`: Appends a new element to the end of the field with the given value.  
- `void add_foo(const char* value)`: Appends a new element to the end of the field using a C-style null-terminated string.  
- `void add_foo(const char* value, int size)`: Like above, but the string size is given explicitly rather than determined by looking for a null-terminator byte.  
- `string* add_foo()`: Adds a new empty string element to the end of the field and returns a pointer to it.  
- `void clear_foo()`: Removes all elements from the field.  After calling this, `foo_size()` will return zero.  
- `const RepeatedPtrField<string>& foo() const`:  Returns the underlying `RepeatedPtrField` that stores the field's elements.  This container class provides STL-like iterators and other methods.  
- `RepeatedPtrField<string>* mutable_foo()`:  Returns a pointer to the underlying mutable `RepeatedPtrField` that stores the field's elements.  This container class provides STL-like iterators and other methods.

### Repeated Enum Fields

Given the enum type:

```protobuf
enum Bar {
  BAR_VALUE = 0;
  OTHER_VALUE = 1;
}
```

For this field definition:

```protobuf
repeated Bar foo = 1; 
```

The compiler will generate the following accessor methods:

- `int foo_size() const`: Returns the number of elements currently in the field.  
- `Bar foo(int index) const`: Returns the element at the given zero-based index. Calling this method  with index outside of [0, foo_size()) yields undefined behavior.  
- `void set_foo(int index, Bar value)`: Sets the value of the element at the given zero-based index.  In debug mode (i.e. NDEBUG is not defined), if `value` does not match any of the values defined for `Bar`, this method will abort the process.  
- `void add_foo(Bar value)`: Appends a new element to the end of the field with the given value.  In debug mode (i.e. NDEBUG is not defined), if `value` does not match any of the values defined for `Bar`, this method will abort the process.  
- `void clear_foo()`: Removes all elements from the field.  After calling this, `foo_size()` will return zero.  
- `const RepeatedField<int>& foo() const`:  Returns the underlying `RepeatedField` that stores the field's elements.  This container class provides STL-like iterators and other methods.  
- `RepeatedField<int>* mutable_foo()`:  Returns a pointer to the underlying mutable `RepeatedField` that stores the field's elements.  This container class provides STL-like iterators and other methods.

### Repeated Embedded Message Fields

Given the message type:

```protobuf
message Bar {} 
```

For this field definitions:

```protobuf
repeated Bar foo = 1; 
```

The compiler will generate the following accessor methods:

- `int foo_size() const`: Returns the number of elements currently in the field.  
- `const Bar& foo(int index) const`: Returns the element at the given zero-based index. Calling this method  with index outside of [0, foo_size()) yields undefined behavior.  
- `Bar* mutable_foo(int index)`: Returns a pointer to the mutable `Bar` object that stores the value of the element at the given zero-based  index.  Calling this method with index outside of [0, foo_size()) yields undefined behavior.  
- `Bar* add_foo()`: Adds a new element to the end of the field and returns a pointer to it.  The returned `Bar` is mutable and will have none of its fields set (i.e. it will be identical to a newly-allocated `Bar`).  
- `void clear_foo()`: Removes all elements from the field.  After calling this, `foo_size()` will return zero.  
- `const RepeatedPtrField<Bar>& foo() const`:  Returns the underlying `RepeatedPtrField` that stores the field's elements.  This container class provides STL-like iterators and other methods.  
- `RepeatedPtrField<Bar>* mutable_foo()`:  Returns a pointer to the underlying mutable `RepeatedPtrField` that stores the field's elements.  This container class provides STL-like iterators and other methods.

### Oneof Numeric Fields

For this [oneof](https://developers.google.com/protocol-buffers/docs/proto#oneof) field definition:

```protobuf
oneof oneof_name {
    int32 foo = 1;
    ...
}
```

The compiler will generate the following accessor methods:

- `bool has_foo() const` (proto2 only): Returns `true` if oneof case is `kFoo`.
- `int32 foo() const`: Returns the current value of the field if oneof case is `kFoo`. Otherwise, returns the default value.
- `void set_foo(int32 value):`
  - If any other oneof field in the same oneof is set, calls `clear_oneof_name()`.
  - Sets the value of this field and sets the oneof case to `kFoo`.
  - `has_foo()` (proto2 only) will return true, `foo()` will return `value`, and `oneof_name_case()` will return `kFoo`.
- `void clear_foo():`
  - Nothing will be changed if oneof case is not `kFoo`.
  - If oneof case is `kFoo`, clears the value of the field and oneof case. `has_foo()` (proto2 only) will return `false`, `foo()` will return the default value and `oneof_name_case()` will return `ONEOF_NAME_NOT_SET`.

For other numeric field types (including `bool`),` int32` is replaced with the corresponding C++ type according to the [scalar value types table](https://developers.google.com/protocol-buffers/docs/reference/language.shtml#scalar).

### Oneof String Fields

For any of these [oneof](https://developers.google.com/protocol-buffers/docs/proto#oneof) field definitions:

```protobuf
oneof oneof_name {
    string foo = 1;
    …
}
oneof oneof_name {
    bytes foo = 1;
    ….
}
```

The compiler will generate the following accessor methods:

- `bool has_foo() const` (proto2 only): Returns `true` if the oneof case is `kFoo`.
- `const string& foo() const`: Returns the current value of the field if the oneof case is `kFoo`. Otherwise, returns the default value.
- `void set_foo(const string& value): `
  - If any other oneof field in the same oneof is set, calls `clear_oneof_name()`.
  - Sets the value of this field and sets the oneof case to `kFoo`.
  - `has_foo()` (proto2 only) will return `true`, `foo()` will return a copy of `value` and `oneof_name_case()` will return `kFoo`.
- `void set_foo(const char* value):`
  - If any other oneof field in the same oneof is set, calls `clear_oneof_name()`.
  - Sets the value of the field using a C-style null-terminated string and set the oneof case to `kFoo`.
  - `has_foo()` (proto2 only) will return `true`, `foo()` will return a copy of `value` and `oneof_name_case()` will return `kFoo`.
- `void set_foo(const char* value, int size)`: 
  - Like above, but the string size is given explicitly rather than determined by looking for a null-terminator byte.
- `string* mutable_foo():`
  - If any other oneof field in the same oneof is set, calls `clear_oneof_name()`.
  - Sets the oneof case to `kFoo` and returns a pointer to the mutable string object that stores the field's value. If the oneof case was not `kFoo` prior to the call, then the returned string will be empty (not the default value). 
  - `has_foo()` (proto2 only) will return `true`, `foo()` will return whatever value is written into the given string and `oneof_name_case()` will return `kFoo`.
- `void clear_foo():`
  - If the oneof case is not `kFoo`, nothing will be changed .
  - If the oneof case is `kFoo`, frees the field and clears the oneof case . `has_foo()` (proto2 only) will return `false`, `foo()` will return the default value, and `oneof_name_case()` will return `ONEOF_NAME_NOT_SET`.
- `void set_allocated_foo(string* value):`
  - Calls `clear_oneof_name()`.
  - If the string pointer is not `NULL`: Sets the string object to the field and sets the oneof case to `kFoo`. The message takes ownership of the allocated string object, `has_foo()` (proto2 only) will return `true` and `oneof_name_case()` will return `kFoo`.
  - If the string pointer is `NULL`, `has_foo()` (proto2 only) will return `false` and `oneof_name_case()` will return `ONEOF_NAME_NOT_SET`.
- `string* release_foo():`
  - Returns `NULL` if oneof case is not `kFoo`.
  - Clears the oneof case, releases the ownership of the field and  returns the pointer of the string object. After calling this, caller  takes the ownership of the allocated string object, `has_foo()` (proto2 only) will return false, `foo()` will return the default value, and `oneof_name_case()` will return `ONEOF_NAME_NOT_SET`.

### Oneof Enum Fields

Given the enum type:

```protobuf
enum Bar {
  BAR_VALUE = 0;
  OTHER_VALUE = 1;
}
```

For the [oneof](https://developers.google.com/protocol-buffers/docs/proto#oneof) field definition:

```protobuf
oneof oneof_name {
    Bar foo = 1;
    ...
}
```

The compiler will generate the following accessor methods:

- `bool has_foo() const` (proto2 only): Returns `true` if oneof case is `kFoo`.
- `Bar foo() const`: Returns the current value of the field if oneof case is `kFoo`. Otherwise, returns the default value.
- `void set_foo(Bar value):`
  - If any other oneof field in the same oneof is set, calls `clear_oneof_name()`.
  - Sets the value of this field and sets the oneof case to `kFoo`.
  - `has_foo()` (proto2 only) will return `true`, `foo()` will return `value` and `oneof_name_case()` will return `kFoo`.
  - In debug mode (i.e. NDEBUG is not defined), if `value` does not match any of the values defined for `Bar`, this method will abort the process.
- `void clear_foo():`
  - Nothing will be changed if the oneof case is not `kFoo`.
  - If the oneof case is `kFoo`, clears the value of the field and the oneof case. `has_foo()` (proto2 only) will return `false`, `foo()` will return the default value and `oneof_name_case()` will return `ONEOF_NAME_NOT_SET`.

### Oneof Embedded Message Fields

Given the message type:

```protobuf
message Bar {}
```

For the [oneof](https://developers.google.com/protocol-buffers/docs/proto#oneof) field definition:

```protobuf
oneof oneof_name {
    Bar foo = 1;
    ...
}
```

The compiler will generate the following accessor methods:

- `bool has_foo() const`: Returns true if oneof case is `kFoo`.
- `const Bar& foo() const`: Returns the current value of the field if oneof case is `kFoo`. Otherwise, returns `Bar::default_instance()`.
- `Bar* mutable_foo():`
  - If any other oneof field in the same oneof is set, calls `clear_oneof_name()`.
  - Sets the oneof case to `kFoo` and returns a pointer to the mutable Bar object that stores the field's value. If the oneof case was not `kFoo` prior to the call, then the returned Bar will have none of its fields set (i.e. it will be identical to a newly-allocated Bar).
  - After calling this, `has_foo()` will return `true`, `foo()` will return a reference to the same instance of `Bar` and `oneof_name_case()` will return `kFoo`.
- `void clear_foo():`
  - Nothing will be changed if the oneof case is not `kFoo`.
  - If the oneof case equals `kFoo`, frees the field and clears the oneof case. `has_foo()` will return `false`, `foo()` will return the default value and `oneof_name_case()` will return `ONEOF_NAME_NOT_SET`.
- `void set_allocated_foo(Bar* bar):`
  - Calls `clear_oneof_name()`.
  - If the `Bar` pointer is not `NULL`: Sets the `Bar` object to the field and sets the oneof case to `kFoo`. The message takes ownership of the allocated `Bar` object, has_foo() will return true and oneof_name_case() will return `kFoo`.
  - If the pointer is `NULL`, `has_foo()` will return `false` and `oneof_name_case()` will return `ONEOF_NAME_NOT_SET`. (The behavior is like calling `clear_oneof_name()`)
- `Bar* release_foo():`
  - Returns `NULL` if oneof case is not `kFoo`.
  - If the oneof case is `kFoo`, clears the oneof case, releases the ownership of the field and returns the pointer of the `Bar` object. After calling this, caller takes the ownership of the allocated `Bar` object, `has_foo()` will return `false`, `foo()` will return the default value and `oneof_name_case()` will return `ONEOF_NAME_NOT_SET`.

### Map Fields

For this map field definition:

```protobuf
map<int32, int32> weight = 1;
```

The compiler will generate the following accessor methods:

- `const google::protobuf::Map<int32, int32>& weight();`: Returns an immutable `Map`.
- `google::protobuf::Map<int32, int32>* mutable_weight();`: Returns a mutable `Map`.

A `google::protobuf::Map` is a  special container type used in protocol buffers to store map fields. As  you can see from its interface below, it uses a commonly-used subset of `std::map` and `std::unordered_map` methods.

```c++
template<typename Key, typename T> {
class Map {
  // Member types
  typedef Key key_type;
  typedef T mapped_type;
  typedef MapPair< Key, T > value_type;

  // Iterators
  iterator begin();
  const_iterator begin() const;
  const_iterator cbegin() const;
  iterator end();
  const_iterator end() const;
  const_iterator cend() const;
  // Capacity
  int size() const;
  bool empty() const;

  // Element access
  T& operator[](const Key& key);
  const T& at(const Key& key) const;
  T& at(const Key& key);

  // Lookup
  int count(const Key& key) const;
  const_iterator find(const Key& key) const;
  iterator find(const Key& key);

  // Modifiers
  pair<iterator, bool> insert(const value_type& value);
  template<class InputIt>
  void insert(InputIt first, InputIt last);
  size_type erase(const Key& Key);
  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);
  void clear();

  // Copy
  Map(const Map& other);
  Map& operator=(const Map& other);
}
```

The easiest way to add data is to use normal map syntax, for example:

```c++
std::unique_ptr<ProtoName> my_enclosing_proto(new ProtoName); (*my_enclosing_proto->mutable_weight())[my_key] = my_value; 
```

`pair<iterator, bool> insert(const value_type& value)` will implicitly cause a deep copy of the `value_type` instance. The most efficient way to insert a new value into a `google::protobuf::Map` is as follows:

```C++
T& operator[](const Key& key): map[new_key] = new_mapped;
```

#### Using `google::protobuf::Map` with standard maps

`google::protobuf::Map` supports the same iterator API as `std::map` and `std::unordered_map`. If you don't want to use `google::protobuf::Map` directly, you can convert a `google::protobuf::Map` to a standard map by doing the following:

```c++
std::map<int32, int32> standard_map(message.weight().begin(),                  message.weight().end());
```

Note that this will make a deep copy of the entire map.

You can also construct a `google::protobuf::Map` from a standard map as follows:

```c++
google::protobuf::Map<int32, int32> weight(standard_map.begin(), standard_map.end());
```

#### Parsing unknown values

On the wire, a .proto map is equivalent to a map entry message for each  key/value pair, while the map itself is a repeated field of map entries. Like ordinary message types, it's possible for a parsed map entry  message to have unknown fields: for example a field of type `int64` in a map defined as `map<int32, string>`.

If there are unknown fields in the wire format of a map entry message, they will be discarded.

If there is an unknown enum value in the wire format of a map  entry message, it's handled differently in proto2 and proto3. In proto2, the whole map entry message is put into the unknown field set of the  containing message. In proto3, it is put into a map field as if it is a  known enum value.

## Any

Given an [`Any`](https://developers.google.com/protocol-buffers/docs/proto3#any) field like this:

```protobuf
import "google/protobuf/any.proto";

message ErrorStatus {
  string message = 1;
  google.protobuf.Any details = 2;
}
```

在我们生成的代码中，细节字段的getter返回一个`google::protobuf::Any`.的实例。这提供了以下特殊方法来打包和解包Any的值

```c++
class Any {
 public:
  // Packs the given message into this Any using the default type URL
  // prefix “type.googleapis.com”. Returns false if serializing the message failed.
  bool PackFrom(const google::protobuf::Message& message);

  // Packs the given message into this Any using the given type URL
  // prefix. Returns false if serializing the message failed.
  bool PackFrom(const google::protobuf::Message& message,
                const string& type_url_prefix);

  // Unpacks this Any to a Message. Returns false if this Any
  // represents a different protobuf type or parsing fails.
  bool UnpackTo(google::protobuf::Message* message) const;

  // Returns true if this Any represents the given protobuf type.
  template<typename T> bool Is() const;
}
```

## Oneof

Given a oneof definition like this:

```protobuf
oneof oneof_name {
    int32 foo_int = 4;
    string foo_string = 9;
    ...
}
```

The compiler will generate the following C++ enum type:

```c++
enum OneofNameCase {
  kFooInt = 4,
  kFooString = 9,
  ONEOF_NAME_NOT_SET = 0
}
```

此外，它将生成以下方法：

- `OneofNameCase oneof_name_case() const`: Returns the enum indicating which field is set. Returns `ONEOF_NAME_NOT_SET` if none of them is set.
- `void clear_oneof_name()`: Frees the object if the oneof field set uses a pointer (Message or String), and sets the oneof case to `ONEOF_NAME_NOT_SET`.

## Enumerations

Given an enum definition like:

```protobuf
enum Foo {
  VALUE_A = 0;
  VALUE_B = 5;
  VALUE_C = 1234;
}
```

协议缓冲编译器将生成一个名为Foo的c++枚举类型，它具有相同的一组值。此外，编译器将生成以下函数:

- `const EnumDescriptor* Foo_descriptor()`: Returns the type's descriptor, which contains information about what values this enum type defines.  
- `bool Foo_IsValid(int value)`: Returns `true` if the given numeric value matches one of `Foo`'s defined values.  In the above example, it would return `true` if the input were 0, 5, or 1234.  
- `const string& Foo_Name(int value)`: Returns the name for given numeric value.  Returns an empty string if  no such value exists.  If multiple values have this number, the first  one defined is returned.  In the above example, `Foo_Name(5)` would return `"VALUE_B"`.  
- `bool Foo_Parse(const string& name, Foo* value)`: If `name` is a valid value name for this enum, assigns that value into `value` and returns true.  Otherwise returns false.  In the above example, `Foo_Parse("VALUE_C", &some_foo)` would return true and set `some_foo` to 1234.  
- `const Foo Foo_MIN`: the smallest valid value of the enum (VALUE_A in the example).  
- `const Foo Foo_MAX`: the largest valid value of the enum (VALUE_C in the example).  
- `const int Foo_ARRAYSIZE`: always defined as `Foo_MAX + 1`.

注：**Be careful when casting integers to proto2 enums.**  If an integer is cast to a proto2 enum value, the integer *must* be one of the valid values for that enum, or the results may be undefined.  If in doubt, use the generated `Foo_IsValid()` function to test if the cast is valid.  Setting an enum-typed field of a proto2 message to an invalid value may cause an assertion failure.  If  an invalid enum value is read when parsing a proto2 message, it will be  treated as an [unknown field](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.unknown_field_set). These semantics have been changed in proto3. It's safe to cast any  integer to a proto3 enum value as long as it fits into int32. Invalid  enum values will also be kept when parsing a proto3 message and returned by enum field accessors.

**Be careful when using proto3 enums in switch statements.** Proto3 enums are open enum types with possible values outside the range of specified symbols. Unrecognized enum values will be kept when  parsing a proto3 message and returned by the enum field accessors.  A  switch statement on a proto3 enum without a default case will not be  able to catch all cases even if all the known fields are listed.  This  could lead to unexpected behavior including data corruption and runtime  crashes. **Always add a default case or explicitly call `Foo_IsValid(int)` outside of the switch to handle unknown enum values.**

You can define an enum inside a message type.  In this case, the  protocol buffer compiler generates code that makes it appear that the  enum type itself was declared nested inside the message's class.  The `Foo_descriptor()` and `Foo_IsValid()` functions are declared as static methods.  In reality, the enum type  itself and its values are declared at the global scope with mangled  names, and are imported into the class's scope with a typedef and a  series of constant definitions.  This is done only to get around  problems with declaration ordering.  Do not depend on the mangled  top-level names; pretend the enum really is nested in the message class.

## Extensions (proto2 only)

Given a message with an extension range:

```protobuf
message Foo {
  extensions 100 to 199;
}
```

The protocol buffer compiler will generate some additional methods for `Foo`:  `HasExtension()`, `ExtensionSize()`, `ClearExtension()`, `GetExtension()`, `SetExtension()`, `MutableExtension()`, `AddExtension()`, `SetAllocatedExtension()` and `ReleaseExtension()`.  Each of these methods takes, as its first parameter, an extension  identifier (described below), which identifies an extension field.  The  remaining parameters and the return value are exactly the same as those  for the corresponding accessor methods that would be generated for a  normal (non-extension) field of the same type as the extension  identifier.  (`GetExtension()` corresponds to the accessors with no special prefix.)

Given an extension definition:

```protobuf
extend Foo {
  optional int32 bar = 123;
  repeated int32 repeated_bar = 124;
}
```

For the singular extension field `bar`, the protocol buffer compiler generates an "extension identifier" called `bar`, which you can use with `Foo`'s extension accessors to access this extension, like so:

```c++
Foo foo;
assert(!foo.HasExtension(bar));
foo.SetExtension(bar, 1);
assert(foo.HasExtension(bar));
assert(foo.GetExtension(bar) == 1);
foo.ClearExtension(bar);
assert(!foo.HasExtension(bar));
```

Similarly, for the repeated extension field `repeated_bar`, the compiler generates an extension identifier called `repeated_bar`, which you can also use with `Foo`'s extension accessors:

```c++
Foo foo;
for (int i = 0; i < kSize; ++i) {
  foo.AddExtension(repeated_bar, i)
}
assert(foo.ExtensionSize(repeated_bar) == kSize)
for (int i = 0; i < kSize; ++i) {
  assert(foo.GetExtension(repeated_bar, i) == i)
}
```

(The exact implementation of extension identifiers is complicated and involves magical use of templates – however, you don't need to worry  about how extension identifiers work to use them.)

Extensions can be declared nested inside of another type.  For example, a common pattern is to do something like this:

```protobuf
message Baz {
  extend Foo {
    optional Baz foo_ext = 124;
  }
}
```

In this case, the extension identifier `foo_ext` is declared nested inside `Baz`.  It can be used as follows:

```c++
Foo foo;
Baz* baz = foo.MutableExtension(Baz::foo_ext);
FillInMyBaz(baz);
```

## Arena Allocation

Arena allocation is a C++-only feature that helps you optimize your memory  usage and improve performance when working with protocol buffers.  Enabling arena allocation in your `.proto` adds additional code for working with arenas to your C++ generated  code. You can find out more about the arena allocation API in the [Arena Allocation Guide](https://developers.google.com/protocol-buffers/docs/reference/arenas).

## Services

If the `.proto` file contains the following line:

```protobuf
option cc_generic_services = true;
```

Then the protocol buffer compiler will generate code based on the service definitions found in the file as described in this section.  However, the generated code may be undesirable as it is not tied to any particular RPC system, and thus requires more levels of indirection than code tailored to one system.  If you do NOT want this code to be generated, add this line to the file:

```protobuf
option cc_generic_services = false;
```

If neither of the above lines are given, the option defaults to `false`, as generic services are deprecated. (Note that prior to 2.4.0, the option defaults to `true`)

RPC systems based on `.proto`-language service definitions should provide [plugins](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.compiler.plugin.pb) to generate code appropriate for the system.  These plugins are likely to require that abstract services are disabled, so that they can generate their own classes of the same names.  Plugins are new in version 2.3.0 (January 2010).

The remainder of this section describes what the protocol buffer compiler generates when abstract services are enabled.

### Interface

Given a service definition:

```protobuf
service Foo {
  rpc Bar(FooRequest) returns(FooResponse);
}
```

协议缓冲编译器将生成一个Foo类来表示这个服务。Foo对于服务定义中定义的每个方法都有一个虚方法。在这种情况下，方法Bar被定义为:

```c++
virtual void Bar(RpcController* controller, const FooRequest* request,
                 FooResponse* response, Closure* done);
```

The parameters are equivalent to the parameters of `Service::CallMethod()`, except that the `method` argument is implied and `request` and `response` specify their exact type.

These generated methods are virtual, but not pure-virtual.  The default implementations simply call `controller->SetFailed()` with an error message indicating that the method is unimplemented, then invoke the `done` callback.  When implementing your own service, you must subclass this  generated service and implement its methods as appropriate.

`Foo` subclasses the `Service` interface.  The protocol buffer compiler automatically generates implementations of the methods of `Service` as follows:

- `GetDescriptor`:  Returns the service's `ServiceDescriptor`.  
- `CallMethod`: Determines which method is being called based on the provided method  descriptor and calls it directly, down-casting the request and response  messages objects to the correct types.  
- `GetRequestPrototype` and `GetResponsePrototype`: Returns the default instance of the request or response of the correct type for the given method.

The following static method is also generated:

- `static ServiceDescriptor descriptor()`: Returns the type's descriptor, which contains information about what  methods this service has and what their input and output types are.

### Stub

The protocol buffer compiler also generates a "stub" implementation of  every service interface, which is used by clients wishing to send  requests to servers implementing the service.  For the `Foo` service (above), the stub implementation `Foo_Stub` will be defined.  As with nested message types, a typedef is used so that `Foo_Stub` can also be referred to as `Foo::Stub`.

`Foo_Stub` is a subclass of `Foo` which also implements the following methods:

- `Foo_Stub(RpcChannel* channel)`: Constructs a new stub which sends requests on the given channel.  
- `Foo_Stub(RpcChannel* channel, ChannelOwnership ownership)`: Constructs a new stub which sends requests on the given channel and possibly owns that channel.  If `ownership` is `Service::STUB_OWNS_CHANNEL` then when the stub object is deleted it will delete the channel as well.  
- `RpcChannel* channel()`: Returns this stub's channel, as passed to the constructor.

The stub additionally implements each of the service's methods as a  wrapper around the channel.  Calling one of the methods simply calls `channel->CallMethod()`.

The Protocol Buffer library does not include an RPC  implementation.  However, it includes all of the tools you need to hook  up a generated service class to any arbitrary RPC implementation of your choice.  You need only provide implementations of `RpcChannel` and `RpcController`.  See the documentation for `service.h` for more information.

## Plugin Insertion Points

[Code generator plugins](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.compiler.plugin.pb) which want to extend the output of the C++ code generator may insert  code of the following types using the given insertion point names.  Each insertion point appears in both the `.pb.cc` file and the `.pb.h` file unless otherwise noted.

- `includes`: Include directives.  
- `namespace_scope`:  Declarations that belong in the file's package/namespace, but not within any particular class.  Appears after all other namespace-scope code.  
- `global_scope`: Declarations that belong at the top level, outside of the file's namespace.  Appears at the very end of the file.  
- `class_scope:TYPENAME`: Member declarations that belong in a message class.  `TYPENAME` is the full proto name, e.g. `package.MessageType`.  Appears after all other public declarations in the class.  This insertion point appears only in the `.pb.h` file.

Do not generate code which relies on private class  members declared by the standard code generator, as these implementation details may change in future versions of Protocol Buffers.              

# C++ Arena Allocation Guide

Arena allocation is a C++-only feature that helps you optimize your memory  usage and improve performance when working with protocol buffers. This page  describes exactly what C++ code the protocol buffer compiler generates in  addition to the code described in the [C++ Generated Code Guide](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated)  when arena allocation is enabled. It assumes that you are familiar with the  material in the [language guide](https://developers.google.com/protocol-buffers/docs/proto) and  the [C++ Generated   Code Guide](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated).

内存分配和释放占协议缓冲区代码中CPU时间的很大一部分。默认情况下，协议缓冲区为每个消息对象、其每个子对象和若干字段类型（如字符串）执行堆分配。这些分配在解析消息和在内存中构建新消息时大量发生，而相关的释放则在释放消息及其子对象树时发生。

基于竞技场的分配被设计用来降低这种性能成本。通过arena分配，新对象从一大块预先分配的内存（称为arena）中分配出来。通过丢弃整个竞技场，可以一次释放所有对象，理想情况下不必运行任何包含对象的析构函数（尽管竞技场在需要时仍然可以维护“析构函数列表”）。通过将对象分配减少到一个简单的指针增量，这使得对象分配更快，并且释放几乎是免费的。Arena分配还提供了更高的缓存效率：当解析消息时，它们更有可能在连续内存中分配，这使得遍历消息更有可能命中热缓存线。

要获得这些好处，您需要了解对象生存期，并找到一个合适的粒度来使用arenas（对于服务器，这通常是每个请求）。您可以了解更多关于如何在使用模式和最佳实践中最大限度地利用arena分配的信息。

## Getting started

You enable arena allocation on a per-`.proto` basis. To do  this, add the following `option` to your `.proto`  file:

```protobuf
option cc_enable_arenas = true;
```

这告诉协议缓冲区编译器生成为文件中的消息使用arena分配所需的额外代码，如下面的示例所示。

```protobuf
#include <google/protobuf/arena.h>
{
  google::protobuf::Arena arena;
  MyMessage* message = google::protobuf::Arena::CreateMessage<MyMessage>(&arena);
  // ...
}
```

``

The message object created by `CreateMessage()` will exist for  as long as `arena` exists, and you should not `delete`  the returned message pointer. All of the message object's internal storage  (with a few exceptions[1](https://developers.google.com/protocol-buffers/docs/reference/arenas#fn1)) and  submessages (for example, submessages in a repeated field within  `MyMessage`) are allocated on the arena as well.

For the most part, the rest of your code will be the same as if you  weren't using arena allocation.

We'll look at the arena API in more detail in the following sections, and  you can see a more extensive [example](https://developers.google.com/protocol-buffers/docs/reference/arenas#example) at the end of the document.

\1. Currently, string fields store their data on the heap  even when the containing message is on the arena. Unknown fields are also  heap-allocated.[↩](https://developers.google.com/protocol-buffers/docs/reference/arenas#ref1) 

## Arena class API

You create message objects on the arena using the [`google::protobuf::Arena`](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.arena)  class. This class implements the following public methods. 

### Constructors

- `Arena()`: Creates a new arena with default parameters,    tuned for average use cases.
- `Arena(const ArenaOptions& options)`: Creates a new    arena that uses the specified allocation options. The options available in    `ArenaOptions` include the ability to use an initial block of    user-provided memory for allocations before resorting to the system    allocator, control over the initial and maximum request sizes for blocks of    memory, and allowing you to pass in custom block allocation and    deallocation function pointers to build freelists and others on top of the    blocks.

### Allocation methods

`template<typename T> static T* CreateMessage(Arena*      arena)`: Creates a new protocol buffer object of message type      `T` on the arena. This message type must be defined in a      `.proto` file with `option cc_enable_arenas =      true;` otherwise, a compile error will result.       

If `arena` is not NULL, the returned message object is      allocated on the arena, its internal storage and submessages (if any)      will be allocated on the same arena, and its lifetime is the same as that      of the arena. The object must not be deleted/freed manually: the arena      owns the message object for lifetime purposes.

If `arena` is NULL, the returned message object is      allocated on the heap, and the caller owns the object upon return.

`template<typename T> static T* Create(Arena* arena,      args...)`: Similar to `CreateMessage()` but lets you      create an object of any class on the arena, not just protocol buffer      message types with `option cc_enable_arenas = true;`: you can      use a protocol buffer message class from a file that doesn't have arena      support enabled, or an arbitrary C++ class. For example, let's say you      have this C++ class:      

```c++
class MyCustomClass {
    MyCustomClass(int arg1, int arg2);
    // ...
};
```

...you can create an instance of it on the arena like this:

```c++
void func() {
    // ...
    google::protobuf::Arena arena;
    MyCustomClass* c = google::protobuf::Arena::Create<MyCustomClass>(&arena, constructor_arg1, constructor_arg2);
    // ...
}
```

`template<typename T> static T* CreateArray(Arena* arena,      size_t n)`: If `arena` is not NULL, this method      allocates raw storage for `n` elements of type `T`      and returns it. The arena owns the returned memory and will free it on      its own destruction. If `arena` is NULL, this method allocates      storage on the heap and the caller receives ownership.       

`T` must have a trivial constructor: constructors are not      called when the array is created on the arena.

### "Owned list" methods

The following methods let you specify that particular objects or  destructors are "owned" by the arena, ensuring that they are deleted or  called when the arena itself is deleted

- `template<typename T> void Own(T* object)`: Adds    `object` to the arena's list of owned heap objects. When the    arena is destroyed, it traverses this list and frees each object using    operator delete, i.e., the system memory allocator. This method is useful    in cases when an object's lifetime should be tied to the arena but, for    whatever reason, the object itself cannot be or was not already allocated    on the arena.
- `template<typename T> void OwnDestructor(T* object)`:    Adds the destructor of `object` to the arena's list of    destructors to call. When the arena is destroyed, it traverses this list    and calls each destructor in turn. It does not attempt to free the    underlying memory of object. This method is useful when an object is    embedded in arena-allocated storage but its destructor will not otherwise    be called, for example because its containing class is a protobuf message    whose destructor won't be called, or because it was manually constructed in    a block allocated by `AllocateArray()`.

### Other methods

- `uint64 SpaceUsed() const`: Returns the total size of the    arena, which is the sum of the sizes of the underlying blocks. This method    is thread-safe; however, if there are concurrent allocations from multiple    threads this method's return value may not include the sizes of those new    blocks.
- `uint64 Reset()`: Destroys the arena's storage, first    calling all registered destructors and freeing all registered heap objects    and then discarding all arena blocks. This teardown procedure is equivalent    to that which occurs when the arena's destructor runs, except the arena is    reusable for new allocations after this method returns. Returns the total    size used by the arena: this information is useful for tuning    performance.
- `template<typename T> Arena* GetArena()`: Returns a    pointer to this arena. Not directly very useful but allows    `Arena` to be used in template instantiations that expect    `GetArena()` methods to be present.

### Thread safety

`google::protobuf::Arena`'s allocation methods are thread-safe,  and the underlying implementation goes to some length to make multithreaded  allocation fast. The `Reset()` method is *not* thread-safe:  the thread performing the arena reset must synchronize with all threads  performing allocations or using objects allocated from that arena first.  

## Generated message class

The following message class members are changed or added when you enable  arena allocation. 

### Message class methods

- `Message(Message&& other)`: If the source message is    not on arena, the move constructor efficiently *moves* all fields    from one message to another without making copies or heap allocations (the    time complexity of this operation is `O(number-of-declared-fields)    `). However, if the source message is on arena, it performs a *deep    copy* of the underlying data. In both cases the source message is left    in a valid but unspecified state.
- `Message& operator=(Message&& other)`: If both    messages are not on arena or are on the *same* arena, the    move-assignment operator efficiently *moves* all fields from one    message to another without making copies or heap allocations (the time    complexity of this operation is `O(number-of-declared-fields)`).    However, if only one message is on arena, or the messages are on different    arenas, it performs a *deep copy* of the underlying data. In both    cases the source message is left in a valid but unspecified state.
- `void Swap(Message* other)`: If both messages to be swapped    are not on arenas or are on the *same* arena, [`Swap()`](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated#message)    behaves as it does without having arena allocation enabled: it efficiently    swaps the message objects' contents, usually via cheap pointer swaps and    avoiding copies at all costs. However, if only one message is on an arena,    or the messages are on different arenas, `Swap()` performs    *deep copies* of the underlying data. This new behavior is necessary    because otherwise the swapped sub-objects could have differing lifetimes,    leading potentially to use-after-free bugs.
- `Message* New(Arena* arena)`: An alternate override for the    standard `New()` method. It allows a new message object of this    type to be created on the given arena. Its semantics are identical to    `Arena::CreateMessage<T>(arena)` if the concrete message    type on which it is called is generated with arena allocation enabled. If    the message type is not generated with arena allocation enabled, then it is    equivalent to an ordinary allocation followed by    `arena->Own(message)` if `arena` is not NULL.
- `Arena* GetArena()`: Returns the arena on which this message    object was allocated, if any.
- `void UnsafeArenaSwap(Message* other)`: Identical to    `Swap()`, except it assumes both objects are on the same arena    (or not on arenas at all) and always uses the efficient pointer-swapping    implementation of this operation. Using this method can improve performance    as, unlike `Swap()`, it doesn't need to check which messages    live on which arena before performing the swap. As the `Unsafe`    prefix suggests, you should only use this method if you are sure the    messages you want to swap aren't on different arenas; otherwise this method    could have unpredictable results.

### Embedded message fields

When you allocate a message object on an arena, its embedded message field  objects (submessages) are automatically owned by the arena as well. How these  message objects are allocated depends on where they are defined:

- If the message type is also defined in a `.proto` file with    arena allocation enabled, the object is allocated on the arena    directly.
- If the message type is from another `.proto` without arena    allocation enabled, the object is heap-allocated but is "owned" by the    parent message's arena. This means that when the arena is destroyed, the    object will be freed along with the objects on the arena itself.

For either of these field definitions:

```protobuf
optional Bar foo = 1;
required Bar foo = 1;
```

The following methods are added or have some special behavior when arena  allocation is enabled. Otherwise, accessor methods just use the [default   behavior](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated#embeddedmessage).

- `Bar* mutable_foo()`: Returns a mutable pointer to the    submessage instance. If the parent object is on an arena then the returned    object will be as well.

- ```
  void set_allocated_foo(Bar* bar)
  ```

  : Takes a new object and      adopts it as the new value for the field. Arena support adds additional      copying semantics to maintain proper ownership when objects cross      arena/arena or arena/heap boundaries:       

  - If the parent object is on the heap and `bar` is on the        heap, or if the parent and message are on the same arena, this method's        behavior is unchanged.
  - If the parent is on an arena and `bar` is on the heap,        the parent message adds `bar` to its arena's ownership list        with `arena->Own()`.
  - If the parent is on an arena and `bar` is on a different        arena, this method makes a copy of message and takes the copy as the        new field value.

- ```
  Bar* release_foo()
  ```

  : Returns the existing submessage instance      of the field, if set, or a NULL pointer if not set, releasing ownership      of this instance to the caller and clearing the parent message's field.      Arena support adds additional copying semantics to maintain the contract      that the returned object is always 

  heap-allocated

  :       

  - If the parent message is on an arena, this method will make a copy        of the submessage on the heap, clear the field value, and return the        copy.
  - If the parent message is on the heap, the method behavior is        unchanged.

- `void unsafe_arena_set_allocated_foo(Bar* bar)`: Identical    to `set_allocated_foo`, but assumes both parent and submessage    are on the same arena. Using this version of the method can improve    performance as it doesn't need to check whether the messages are on a    particular arena or the heap.  See    [allocated/release patterns](https://developers.google.com/protocol-buffers/docs/reference/arenas#set-allocatedadd-allocatedrelease)    for details on safe ways to use this.    

- `Bar* unsafe_arena_release_foo()`: Similar to    `release_foo()`, but skips all ownership checking. See    [allocated/release patterns](https://developers.google.com/protocol-buffers/docs/reference/arenas#set-allocatedadd-allocatedrelease)    for details on safe ways to use this.    

### String fields

Currently, string fields store their data on the heap even when their  parent message is on the arena. Because of this, string accessor methods  use the [default   behavior](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated#string) even when arena allocation is enabled.

When arenas are enabled, string and bytes fields generate  `unsafe_arena_release_*field*()` and  `unsafe_arena_set_allocated_*field*()` methods. Note that  these methods are **deprecated** and will be removed in a future  release. Those methods were added by mistake and offer no performance  advantage over their safe equivalents.

### Repeated fields

Repeated fields allocate their internal array storage on the arena when  the containing message is arena-allocated, and also allocate their elements  on the arena when these elements are separate objects retained by pointer  (messages or strings). At the message-class level, generated methods for  repeated fields do not change. However, the `RepeatedField` and  `RepeatedPtrField` objects that are returned by accessors do have  new methods and modified semantics when arena support is enabled.

#### Repeated numeric fields

`RepeatedField` objects that contain [primitive types](https://developers.google.com/protocol-buffers/docs/reference/arenas#repeatednumeric) have the following new/changed methods  when arena allocation is enabled:

- `void UnsafeArenaSwap(RepeatedField* other)`: Performs a    swap of `RepeatedField` contents without validating that this    repeated field and other are on the same arena. If they are not, the two    repeated field objects must be on arenas with equivalent lifetimes. The    case where one is on an arena and one is on the heap is checked and    disallowed.
- `void Swap(RepeatedField* other)`: Checks each repeated    field object's arena, and if one is on an arena while one is on the heap or    if both are on arenas but on different ones, the underlying arrays are    copied before the swap occurs. This means that after the swap, each    repeated field object holds an array on its own arena or heap, as    appropriate.

#### Repeated embedded message fields

`RepeatedPtrField` objects that contain [messages](https://developers.google.com/protocol-buffers/docs/reference/arenas#repeatedmessage) have the following new/changed methods when  arena allocation is enabled.

- `void UnsafeArenaSwap(RepeatedPtrField* other)`: Performs a    swap of `RepeatedPtrField` contents without validating that this    repeated field and other have the same arena pointer. If they do not, the    two repeated field objects must have arena pointers with equivalent    lifetimes. The case where one has a non-NULL arena pointer and one has a    NULL arena pointer is checked and disallowed.
- `void Swap(RepeatedPtrField* other)`: Checks each repeated    field object's arena pointer, and if one is non-NULL (contents on arena)    while one is NULL (contents on heap) or if both are non-NULL but have    different values, the underlying arrays and their pointed-to objects are    copied before the swap occurs. This means that after the swap, each    repeated field object holds an array on its own arena or on the heap, as    appropriate.
- `void AddAllocated(SubMessageType* value)`: Checks that the    provided message object is on the same arena as the repeated field's arena    pointer. If it is on the same arena, then the object pointer is added    directly to the underlying array. Otherwise, a copy is made, the original    is freed if it was heap-allocated, and the copy is placed on the array.    This maintains the invariant that all objects pointed to by a repeated    field are in the same ownership domain (heap or specific arena) as    indicated by the repeated field's arena pointer.
- `SubMessageType* ReleaseLast()`: Returns a heap-allocated    message equivalent to the last message in the repeated field, removing it    from the repeated field. If the repeated field itself has a NULL arena    pointer (and thus, all of its pointed-to messages are heap-allocated), then    this method simply returns a pointer to the original object. Otherwise, if    the repeated field has a non-NULL arena pointer, this method makes a copy    that is heap-allocated and returns that copy. In both cases, the caller    receives ownership of a heap-allocated object and is responsible for    deleting the object.
- `void UnsafeArenaAddAllocated(SubMessageType* value)`: Like    `AddAllocated()`, but does not perform heap/arena checks or any    message copies. It adds the provided pointer directly to the internal array    of pointers for this repeated field.  See    [allocated/release patterns](https://developers.google.com/protocol-buffers/docs/reference/arenas#set-allocatedadd-allocatedrelease)    for details on safe ways to use this.    
- `SubMessageType* UnsafeArenaReleaseLast()`: Like    `ReleaseLast()` but performs no copies, even if the repeated    field has a non-NULL arena pointer. Instead, it directly returns the    pointer to the object as it was in the repeated field.  See    [allocated/release patterns](https://developers.google.com/protocol-buffers/docs/reference/arenas#set-allocatedadd-allocatedrelease)    for details on safe ways to use this.    
- `void ExtractSubrange(int start, int num, SubMessageType**    elements)`: Removes `num` elements from the repeated    field, starting from index `start`, and returns them in    `elements` if it is not NULL. If the repeated field is on an    arena, and elements are being returned, the elements are copied to the heap    first. In both cases (arena or no arena), the caller owns the returned    objects on the heap.
- `void UnsafeArenaExtractSubrange(int start, int num,    SubMessageType** elements)`: Removes `num` elements from    the repeated field, starting from index `start`, and returns    them in `elements` if it is not NULL. Unlike    `ExtractSubrange()`, this method never copies the extracted    elements. See    [allocated/release patterns](https://developers.google.com/protocol-buffers/docs/reference/arenas#set-allocatedadd-allocatedrelease)    for details on safe ways to use this.    

#### Repeated string fields

Repeated fields of strings have the same new methods and modified  semantics as repeated fields of messages, because they also maintain their  underlying objects (namely, strings) by pointer reference.

## Usage patterns and best practices

When using arena-allocated messages, several usage patterns can result in  unintended copies or other negative performance effects. You should be aware  of the following common patterns that may need to be altered when adapting  code for arenas. (Note that we have taken care in the API design to ensure  that correct behavior still occurs — but higher-performance solutions  may require some reworking.)

### Unintended copies

Several methods that never create object copies when not using arena  allocation may end up doing so when arena support is enabled. These unwanted  copies can be avoided if you make sure that your objects are allocated  appropriately and/or use provided arena-specific method versions, as  described in more detail below.

#### Set Allocated/Add Allocated/Release

By default, the `release_*field*()` and  `set_allocated_*field*()` methods (for singular message  fields), and the `ReleaseLast()` and `AddAllocated()`()  methods (for repeated message fields) allow user code to directly attach and  detach submessages, passing ownership of pointers without copying any  data.

However, when the parent message is on an arena, these methods now  sometimes need to copy the passed in or returned object to maintain  compatibility with existing ownership contracts. More specifically, methods  that take ownership (`set_allocated_*field*()` and  `AddAllocated()`) may copy data if the parent is on an arena and  the new subobject is not, or vice versa, or they are on different arenas.  Methods that release ownership (`release_*field*()` and  `ReleaseLast()`) may copy data if the parent is on the arena,  because the returned object must be on the heap, by contract.

To avoid such copies, we have added corresponding "unsafe arena" versions  of these methods where copies are **never performed**:  `unsafe_arena_set_allocated_*field*()`,  `unsafe_arena_release_*field*()`,  `UnsafeArenaAddAllocated()`, and `UnsafeArenaRelease()`  for singular and repeated fields, respectively. These methods should be used  only when you know they are safe to do so.  There are two common patterns for  these methods:

- Moving messages trees between parts of the same arena.  Note that the    messages must be on the same arena for this case to be safe.
- Temporarily loaning an owned message to a tree to avoid copies. Pairing    an unsafe *add*/*set* method with an unsafe *release*    method performs the loan in the cheapest way possible regardless of how    either message is owned (this pattern works when they are on the same    arena, different arena, or no arena at all).  Note that between the unsafe    *add*/*set* and its corresponding *release*, the    the borrower must not be swapped, moved, cleared or destroyed; the loaned    message must not be swapped or moved; the loaned message must not be cleared    or released by the borrower; and the loaned message must not be    destroyed.

Here's an example of how you can avoid unnecessary copies with these  methods. Let's say you have created the following messages on an arena.

```c++
Arena* arena = new google::protobuf::Arena();
MyFeatureMessage* arena_message_1 =
  google::protobuf::Arena::CreateMessage<MyFeatureMessage>(arena);
arena_message_1->mutable_nested_message()->set_feature_id(11);

MyFeatureMessage* arena_message_2 =
  google::protobuf::Arena::CreateMessage<MyFeatureMessage>(arena);
```

The following code makes inefficient usage of the  `release_...()` API:

```c++
arena_message_2->set_allocated_nested_message(arena_message_1->release_nested_message());

arena_message_1->release_message(); // returns a copy of the underlying nested_message and deletes underlying pointer
```

Using the "unsafe arena" version instead avoids the copy:

```c++
arena_message_2->unsafe_arena_set_allocated_nested_message(  arena_message_1->unsafe_arena_release_nested_message());
```

You can find out more about these methods in the [Embedded message fields](https://developers.google.com/protocol-buffers/docs/reference/arenas#arenaembeddedmessage) section above.

#### Swap

When two messages' contents are swapped with `Swap()`, the  underlying subobjects may be copied if the two messages live on different  arenas, or if one is on the arena and the other is on the heap. If you want  to avoid this copy and either (i) know that the two messages are on the same  arena or different arenas but the arenas have equivalent lifetimes, or (ii)  know that the two messages are on the heap, you can use a new method,  `UnsafeArenaSwap()`. This method both avoids the overhead of  performing the arena check and avoids the copy if one would have  occurred.

For example, the following code incurs a copy in the `Swap()`  call:

```c++
MyFeatureMessage* message_1 =
  google::protobuf::Arena::CreateMessage<MyFeatureMessage>(arena);
message_1->mutable_nested_message()->set_feature_id(11);

MyFeatureMessage* message_2 = new MyFeatureMessage;
message_2->mutable_nested_message()->set_feature_id(22);

message_1->Swap(message_2); // Inefficient swap!
```

To avoid the copy in this code, you allocate `message_2` on the  same arena as `message_1`:

```c++
MyFeatureMessage* message_2 =  google::protobuf::Arena::CreateMessage<MyFeatureMessage>(arena);
```

### Embedded message fields and arena-enable options

Each `.proto` file has its own "feature switch" for arena  support. If `cc_enable_arenas` is not set in a given  `.proto` file, the types defined in that file will not be stored  on the arena, even if some other type includes a submessage that has a type  defined in that file. In other words, `cc_enable_arenas` is not  transitive. Rather, submessages of an arena-capable message that do not  themselves have arena support will always be stored on the heap, and will be  added to the parent message's arena's `Own()` list so that their  lifetimes are tied to the arena's lifetime.

The reason for this restriction is that adding arena support adds some  overhead in the case that arenas are not used because of the extra generated  code, so we choose (for now) not to enable arena support globally.  Furthermore, for type- and API-compatibility reasons, we can have only one  C++ generated class per proto message type, so we cannot generate with-arena  and without-arena versions of a class. In the future, after further  optimization, we may be able to lift this restriction and globally enable  arena support. For now, though, it should be enabled for as many submessage  types as possible to improve performance.

### Granularity

We have found in most application server use cases that an  "arena-per-request" model works well. You may be tempted to divide arena use  further, either to reduce heap overhead (by destroying smaller arenas more  often) or to reduce perceived thread-contention issues. However, the use of  more fine-grained arenas may lead to unintended message copying, as we  describe above. We have also spent effort to optimize the `Arena`  implementation for the multithreaded use-case, so a single arena should be  appropriate for use throughout a request lifetime even if multiple threads process that request.

## Example

Here's a simple complete example demonstrating some of the features of the  arena allocation API.

```protobuf
// my_feature.proto

syntax = "proto2";
import "nested_message.proto";

package feature_package;

option cc_enable_arenas = true;

// NEXT Tag to use: 4
message MyFeatureMessage {
  optional string feature_name = 1;
  repeated int32 feature_data = 2;
  optional NestedMessage nested_message = 3;
};
```

```c++
// nested_message.proto

syntax = "proto2";

package feature_package;

// add cc_enable_arenas on each submessage for
// the best performance when using arenas.
option cc_enable_arenas = true;

// NEXT Tag to use: 2
message NestedMessage {
  optional int32 feature_id = 1;
};
```

Message construction and deallocation:

```c++
#include <google/protobuf/arena.h>

Arena arena;

MyFeatureMessage* arena_message =
   google::protobuf::Arena::CreateMessage<MyFeatureMessage>(&arena);

arena_message->set_feature_name("Proto2 Arena");
arena_message->mutable_feature_data()->Add(2);
arena_message->mutable_feature_data()->Add(4);
arena_message->mutable_nested_message()->set_feature_id(247);
```

# [C++ API](https://developers.google.com/protocol-buffers/docs/reference/cpp#google.protobuf)

## google::protobuf



| Files                                                        |
| ------------------------------------------------------------ |
| `google/protobuf/arena.h`This file defines an [Arena](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.arena#Arena) allocator for better allocation performance. |
| `google/protobuf/descriptor.h`This file contains classes which describe a type of protocol message. |
| `google/protobuf/descriptor.pb.h`Protocol buffer representations of descriptors. |
| `google/protobuf/descriptor_database.h`Interface for manipulating databases of descriptors. |
| `google/protobuf/dynamic_message.h`Defines an implementation of [Message](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.message#Message) which can emulate types which are not known at compile-time. |
| `google/protobuf/map.h`This file defines the map container and its helpers to support protobuf maps. |
| `google/protobuf/message.h`Defines [Message](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.message#Message), the abstract interface implemented by non-lite protocol message objects. |
| `google/protobuf/message_lite.h`Defines [MessageLite](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.message_lite#MessageLite), the abstract interface implemented by all (lite and non-lite) protocol message objects. |
| `google/protobuf/repeated_field.h`[RepeatedField](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.repeated_field#RepeatedField) and [RepeatedPtrField](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.repeated_field#RepeatedPtrField) are used by generated protocol message classes to manipulate repeated fields. |
| `google/protobuf/service.h`DEPRECATED: This module declares the abstract interfaces underlying proto2 RPC services. |
| `google/protobuf/text_format.h`Utilities for printing and parsing protocol messages in a human-readable, text-based format. |
| `google/protobuf/unknown_field_set.h`Contains classes used to keep track of unrecognized fields seen while parsing a protocol message. |

## google::protobuf::io

Auxiliary classes used for I/O. 

The Protocol Buffer library uses the classes in this package to deal with  I/O and encoding/decoding raw bytes. Most users will not need to deal  with this package. However, users who want to adapt the system to work  with their own I/O abstractions – e.g., to allow Protocol Buffers to be  read from a different kind of input stream without the need for a  temporary buffer – should take a closer look. 

| Files                                                        |
| ------------------------------------------------------------ |
| `google/protobuf/io/coded_stream.h`This file contains the [CodedInputStream](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.io.coded_stream#CodedInputStream) and [CodedOutputStream](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.io.coded_stream#CodedOutputStream) classes, which wrap a [ZeroCopyInputStream](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.io.zero_copy_stream#ZeroCopyInputStream) or [ZeroCopyOutputStream](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.io.zero_copy_stream#ZeroCopyOutputStream), respectively, and allow you to read or write individual pieces of data in various formats. |
| `google/protobuf/io/printer.h`Utility class for writing text to a [ZeroCopyOutputStream](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.io.zero_copy_stream#ZeroCopyOutputStream). |
| `google/protobuf/io/tokenizer.h`Class for parsing tokenized text from a [ZeroCopyInputStream](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.io.zero_copy_stream#ZeroCopyInputStream). |
| `google/protobuf/io/zero_copy_stream.h`This file contains the [ZeroCopyInputStream](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.io.zero_copy_stream#ZeroCopyInputStream) and [ZeroCopyOutputStream](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.io.zero_copy_stream#ZeroCopyOutputStream) interfaces, which represent abstract I/O streams to and from which protocol buffers can be read and written. |
| `google/protobuf/io/zero_copy_stream_impl.h`This file contains common implementations of the interfaces defined in [zero_copy_stream.h](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.io.zero_copy_stream) which are only included in the full (non-lite) protobuf library. |
| `google/protobuf/io/zero_copy_stream_impl_lite.h`This file contains common implementations of the interfaces defined in [zero_copy_stream.h](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.io.zero_copy_stream) which are included in the "lite" protobuf library. |

## google::protobuf::util

Utility classes. 

This package contains various utilities for message comparison, JSON conversion, well known types, etc. 

| Files                                                        |
| ------------------------------------------------------------ |
| `google/protobuf/util/field_comparator.h`Defines classes for field comparison. |
| `google/protobuf/util/field_mask_util.h`Defines utilities for the FieldMask well known type. |
| `google/protobuf/util/json_util.h`Utility functions to convert between protobuf binary format and proto3 JSON format. |
| `google/protobuf/util/message_differencer.h`This file defines static methods and classes for comparing Protocol Messages. |
| `google/protobuf/util/time_util.h`Defines utilities for the Timestamp and Duration well known types. |
| `google/protobuf/util/type_resolver.h`Defines a TypeResolver for the Any message. |
| `google/protobuf/util/type_resolver_util.h`Defines utilities for the TypeResolver. |

## google::protobuf::compiler

Implementation of the Protocol Buffer compiler. 

This package contains code for parsing .proto files and generating code  based on them. There are two reasons you might be interested in this  package:

- You want to parse .proto files at runtime. In this case, you should look at [importer.h](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.compiler.importer). Since this functionality is widely useful, it is included in the  libprotobuf base library; you do not have to link against libprotoc.
- You want to write a custom protocol compiler which generates  different kinds of code, e.g. code in a different language which is not  supported by the official compiler. For this purpose, [command_line_interface.h](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.compiler.command_line_interface) provides you with a complete compiler front-end, so all you need to do is write a custom implementation of [CodeGenerator](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.compiler.code_generator#CodeGenerator) and a trivial main() function. You can even make your compiler support  the official languages in addition to your own. Since this functionality is only useful to those writing custom compilers, it is in a separate  library called "libprotoc" which you will have to link against. 

| Files                                                        |
| ------------------------------------------------------------ |
| `google/protobuf/compiler/code_generator.h`Defines the abstract interface implemented by each of the language-specific code generators. |
| `google/protobuf/compiler/command_line_interface.h`Implements the Protocol Compiler front-end such that it may be reused by custom compilers written to support other languages. |
| `google/protobuf/compiler/importer.h`This file is the public interface to the .proto file parser. |
| `google/protobuf/compiler/parser.h`Implements parsing of .proto files to FileDescriptorProtos. |
| `google/protobuf/compiler/plugin.h`Front-end for protoc code generator plugins written in C++. |
| `google/protobuf/compiler/plugin.pb.h`API for protoc plugins. |
| `google/protobuf/compiler/cpp/cpp_generator.h`Generates C++ code for a given .proto file. |
| `google/protobuf/compiler/csharp/csharp_generator.h`Generates C# code for a given .proto file. |
| `google/protobuf/compiler/csharp/csharp_names.h`Provides a mechanism for mapping a descriptor to the fully-qualified name of the corresponding C# class. |
| `google/protobuf/compiler/java/java_generator.h`Generates Java code for a given .proto file. |
| `google/protobuf/compiler/java/java_names.h`Provides a mechanism for mapping a descriptor to the fully-qualified name of the corresponding Java class. |
| `google/protobuf/compiler/js/js_generator.h`Generates JavaScript code for a given .proto file. |
| `google/protobuf/compiler/objectivec/objectivec_generator.h`Generates ObjectiveC code for a given .proto file. |
| `google/protobuf/compiler/objectivec/objectivec_helpers.h`Helper functions for generating ObjectiveC code. |
| `google/protobuf/compiler/python/python_generator.h`Generates Python code for a given .proto file. |
| `google/protobuf/compiler/ruby/ruby_generator.h`Generates Ruby code for a given .proto file. |