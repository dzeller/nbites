// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: PMotion.proto

package messages;

/**
 * Protobuf type {@code messages.HeadMotionCommand}
 */
public final class HeadMotionCommand extends
    com.google.protobuf.GeneratedMessage implements
    // @@protoc_insertion_point(message_implements:messages.HeadMotionCommand)
    HeadMotionCommandOrBuilder {
  // Use HeadMotionCommand.newBuilder() to construct.
  private HeadMotionCommand(com.google.protobuf.GeneratedMessage.Builder<?> builder) {
    super(builder);
    this.unknownFields = builder.getUnknownFields();
  }
  private HeadMotionCommand(boolean noInit) { this.unknownFields = com.google.protobuf.UnknownFieldSet.getDefaultInstance(); }

  private static final HeadMotionCommand defaultInstance;
  public static HeadMotionCommand getDefaultInstance() {
    return defaultInstance;
  }

  public HeadMotionCommand getDefaultInstanceForType() {
    return defaultInstance;
  }

  private final com.google.protobuf.UnknownFieldSet unknownFields;
  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
      getUnknownFields() {
    return this.unknownFields;
  }
  private HeadMotionCommand(
      com.google.protobuf.CodedInputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    initFields();
    int mutable_bitField0_ = 0;
    com.google.protobuf.UnknownFieldSet.Builder unknownFields =
        com.google.protobuf.UnknownFieldSet.newBuilder();
    try {
      boolean done = false;
      while (!done) {
        int tag = input.readTag();
        switch (tag) {
          case 0:
            done = true;
            break;
          default: {
            if (!parseUnknownField(input, unknownFields,
                                   extensionRegistry, tag)) {
              done = true;
            }
            break;
          }
          case 8: {
            int rawValue = input.readEnum();
            messages.HeadMotionCommand.CommandType value = messages.HeadMotionCommand.CommandType.valueOf(rawValue);
            if (value == null) {
              unknownFields.mergeVarintField(1, rawValue);
            } else {
              bitField0_ |= 0x00000001;
              type_ = value;
            }
            break;
          }
          case 18: {
            messages.PositionHeadCommand.Builder subBuilder = null;
            if (((bitField0_ & 0x00000002) == 0x00000002)) {
              subBuilder = posCommand_.toBuilder();
            }
            posCommand_ = input.readMessage(messages.PositionHeadCommand.PARSER, extensionRegistry);
            if (subBuilder != null) {
              subBuilder.mergeFrom(posCommand_);
              posCommand_ = subBuilder.buildPartial();
            }
            bitField0_ |= 0x00000002;
            break;
          }
          case 26: {
            messages.ScriptedHeadCommand.Builder subBuilder = null;
            if (((bitField0_ & 0x00000004) == 0x00000004)) {
              subBuilder = scriptedCommand_.toBuilder();
            }
            scriptedCommand_ = input.readMessage(messages.ScriptedHeadCommand.PARSER, extensionRegistry);
            if (subBuilder != null) {
              subBuilder.mergeFrom(scriptedCommand_);
              scriptedCommand_ = subBuilder.buildPartial();
            }
            bitField0_ |= 0x00000004;
            break;
          }
          case 32: {
            bitField0_ |= 0x00000008;
            timestamp_ = input.readInt64();
            break;
          }
        }
      }
    } catch (com.google.protobuf.InvalidProtocolBufferException e) {
      throw e.setUnfinishedMessage(this);
    } catch (java.io.IOException e) {
      throw new com.google.protobuf.InvalidProtocolBufferException(
          e.getMessage()).setUnfinishedMessage(this);
    } finally {
      this.unknownFields = unknownFields.build();
      makeExtensionsImmutable();
    }
  }
  public static final com.google.protobuf.Descriptors.Descriptor
      getDescriptor() {
    return messages._File_PMotion.internal_static_messages_HeadMotionCommand_descriptor;
  }

  protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return messages._File_PMotion.internal_static_messages_HeadMotionCommand_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            messages.HeadMotionCommand.class, messages.HeadMotionCommand.Builder.class);
  }

  public static com.google.protobuf.Parser<HeadMotionCommand> PARSER =
      new com.google.protobuf.AbstractParser<HeadMotionCommand>() {
    public HeadMotionCommand parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new HeadMotionCommand(input, extensionRegistry);
    }
  };

  @java.lang.Override
  public com.google.protobuf.Parser<HeadMotionCommand> getParserForType() {
    return PARSER;
  }

  /**
   * Protobuf enum {@code messages.HeadMotionCommand.CommandType}
   */
  public enum CommandType
      implements com.google.protobuf.ProtocolMessageEnum {
    /**
     * <code>POS_HEAD_COMMAND = 0;</code>
     *
     * <pre>
     * AKA set_head_command
     * </pre>
     */
    POS_HEAD_COMMAND(0, 0),
    /**
     * <code>SCRIPTED_HEAD_COMMAND = 1;</code>
     */
    SCRIPTED_HEAD_COMMAND(1, 1),
    ;

    /**
     * <code>POS_HEAD_COMMAND = 0;</code>
     *
     * <pre>
     * AKA set_head_command
     * </pre>
     */
    public static final int POS_HEAD_COMMAND_VALUE = 0;
    /**
     * <code>SCRIPTED_HEAD_COMMAND = 1;</code>
     */
    public static final int SCRIPTED_HEAD_COMMAND_VALUE = 1;


    public final int getNumber() { return value; }

    public static CommandType valueOf(int value) {
      switch (value) {
        case 0: return POS_HEAD_COMMAND;
        case 1: return SCRIPTED_HEAD_COMMAND;
        default: return null;
      }
    }

    public static com.google.protobuf.Internal.EnumLiteMap<CommandType>
        internalGetValueMap() {
      return internalValueMap;
    }
    private static com.google.protobuf.Internal.EnumLiteMap<CommandType>
        internalValueMap =
          new com.google.protobuf.Internal.EnumLiteMap<CommandType>() {
            public CommandType findValueByNumber(int number) {
              return CommandType.valueOf(number);
            }
          };

    public final com.google.protobuf.Descriptors.EnumValueDescriptor
        getValueDescriptor() {
      return getDescriptor().getValues().get(index);
    }
    public final com.google.protobuf.Descriptors.EnumDescriptor
        getDescriptorForType() {
      return getDescriptor();
    }
    public static final com.google.protobuf.Descriptors.EnumDescriptor
        getDescriptor() {
      return messages.HeadMotionCommand.getDescriptor().getEnumTypes().get(0);
    }

    private static final CommandType[] VALUES = values();

    public static CommandType valueOf(
        com.google.protobuf.Descriptors.EnumValueDescriptor desc) {
      if (desc.getType() != getDescriptor()) {
        throw new java.lang.IllegalArgumentException(
          "EnumValueDescriptor is not for this type.");
      }
      return VALUES[desc.getIndex()];
    }

    private final int index;
    private final int value;

    private CommandType(int index, int value) {
      this.index = index;
      this.value = value;
    }

    // @@protoc_insertion_point(enum_scope:messages.HeadMotionCommand.CommandType)
  }

  private int bitField0_;
  public static final int TYPE_FIELD_NUMBER = 1;
  private messages.HeadMotionCommand.CommandType type_;
  /**
   * <code>optional .messages.HeadMotionCommand.CommandType type = 1;</code>
   */
  public boolean hasType() {
    return ((bitField0_ & 0x00000001) == 0x00000001);
  }
  /**
   * <code>optional .messages.HeadMotionCommand.CommandType type = 1;</code>
   */
  public messages.HeadMotionCommand.CommandType getType() {
    return type_;
  }

  public static final int POS_COMMAND_FIELD_NUMBER = 2;
  private messages.PositionHeadCommand posCommand_;
  /**
   * <code>optional .messages.PositionHeadCommand pos_command = 2;</code>
   */
  public boolean hasPosCommand() {
    return ((bitField0_ & 0x00000002) == 0x00000002);
  }
  /**
   * <code>optional .messages.PositionHeadCommand pos_command = 2;</code>
   */
  public messages.PositionHeadCommand getPosCommand() {
    return posCommand_;
  }
  /**
   * <code>optional .messages.PositionHeadCommand pos_command = 2;</code>
   */
  public messages.PositionHeadCommandOrBuilder getPosCommandOrBuilder() {
    return posCommand_;
  }

  public static final int SCRIPTED_COMMAND_FIELD_NUMBER = 3;
  private messages.ScriptedHeadCommand scriptedCommand_;
  /**
   * <code>optional .messages.ScriptedHeadCommand scripted_command = 3;</code>
   */
  public boolean hasScriptedCommand() {
    return ((bitField0_ & 0x00000004) == 0x00000004);
  }
  /**
   * <code>optional .messages.ScriptedHeadCommand scripted_command = 3;</code>
   */
  public messages.ScriptedHeadCommand getScriptedCommand() {
    return scriptedCommand_;
  }
  /**
   * <code>optional .messages.ScriptedHeadCommand scripted_command = 3;</code>
   */
  public messages.ScriptedHeadCommandOrBuilder getScriptedCommandOrBuilder() {
    return scriptedCommand_;
  }

  public static final int TIMESTAMP_FIELD_NUMBER = 4;
  private long timestamp_;
  /**
   * <code>optional int64 timestamp = 4;</code>
   */
  public boolean hasTimestamp() {
    return ((bitField0_ & 0x00000008) == 0x00000008);
  }
  /**
   * <code>optional int64 timestamp = 4;</code>
   */
  public long getTimestamp() {
    return timestamp_;
  }

  private void initFields() {
    type_ = messages.HeadMotionCommand.CommandType.POS_HEAD_COMMAND;
    posCommand_ = messages.PositionHeadCommand.getDefaultInstance();
    scriptedCommand_ = messages.ScriptedHeadCommand.getDefaultInstance();
    timestamp_ = 0L;
  }
  private byte memoizedIsInitialized = -1;
  public final boolean isInitialized() {
    byte isInitialized = memoizedIsInitialized;
    if (isInitialized == 1) return true;
    if (isInitialized == 0) return false;

    memoizedIsInitialized = 1;
    return true;
  }

  public void writeTo(com.google.protobuf.CodedOutputStream output)
                      throws java.io.IOException {
    getSerializedSize();
    if (((bitField0_ & 0x00000001) == 0x00000001)) {
      output.writeEnum(1, type_.getNumber());
    }
    if (((bitField0_ & 0x00000002) == 0x00000002)) {
      output.writeMessage(2, posCommand_);
    }
    if (((bitField0_ & 0x00000004) == 0x00000004)) {
      output.writeMessage(3, scriptedCommand_);
    }
    if (((bitField0_ & 0x00000008) == 0x00000008)) {
      output.writeInt64(4, timestamp_);
    }
    getUnknownFields().writeTo(output);
  }

  private int memoizedSerializedSize = -1;
  public int getSerializedSize() {
    int size = memoizedSerializedSize;
    if (size != -1) return size;

    size = 0;
    if (((bitField0_ & 0x00000001) == 0x00000001)) {
      size += com.google.protobuf.CodedOutputStream
        .computeEnumSize(1, type_.getNumber());
    }
    if (((bitField0_ & 0x00000002) == 0x00000002)) {
      size += com.google.protobuf.CodedOutputStream
        .computeMessageSize(2, posCommand_);
    }
    if (((bitField0_ & 0x00000004) == 0x00000004)) {
      size += com.google.protobuf.CodedOutputStream
        .computeMessageSize(3, scriptedCommand_);
    }
    if (((bitField0_ & 0x00000008) == 0x00000008)) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(4, timestamp_);
    }
    size += getUnknownFields().getSerializedSize();
    memoizedSerializedSize = size;
    return size;
  }

  private static final long serialVersionUID = 0L;
  @java.lang.Override
  protected java.lang.Object writeReplace()
      throws java.io.ObjectStreamException {
    return super.writeReplace();
  }

  public static messages.HeadMotionCommand parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static messages.HeadMotionCommand parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static messages.HeadMotionCommand parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static messages.HeadMotionCommand parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static messages.HeadMotionCommand parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return PARSER.parseFrom(input);
  }
  public static messages.HeadMotionCommand parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return PARSER.parseFrom(input, extensionRegistry);
  }
  public static messages.HeadMotionCommand parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return PARSER.parseDelimitedFrom(input);
  }
  public static messages.HeadMotionCommand parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return PARSER.parseDelimitedFrom(input, extensionRegistry);
  }
  public static messages.HeadMotionCommand parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return PARSER.parseFrom(input);
  }
  public static messages.HeadMotionCommand parseFrom(
      com.google.protobuf.CodedInputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return PARSER.parseFrom(input, extensionRegistry);
  }

  public static Builder newBuilder() { return Builder.create(); }
  public Builder newBuilderForType() { return newBuilder(); }
  public static Builder newBuilder(messages.HeadMotionCommand prototype) {
    return newBuilder().mergeFrom(prototype);
  }
  public Builder toBuilder() { return newBuilder(this); }

  @java.lang.Override
  protected Builder newBuilderForType(
      com.google.protobuf.GeneratedMessage.BuilderParent parent) {
    Builder builder = new Builder(parent);
    return builder;
  }
  /**
   * Protobuf type {@code messages.HeadMotionCommand}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessage.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:messages.HeadMotionCommand)
      messages.HeadMotionCommandOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return messages._File_PMotion.internal_static_messages_HeadMotionCommand_descriptor;
    }

    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return messages._File_PMotion.internal_static_messages_HeadMotionCommand_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              messages.HeadMotionCommand.class, messages.HeadMotionCommand.Builder.class);
    }

    // Construct using messages.HeadMotionCommand.newBuilder()
    private Builder() {
      maybeForceBuilderInitialization();
    }

    private Builder(
        com.google.protobuf.GeneratedMessage.BuilderParent parent) {
      super(parent);
      maybeForceBuilderInitialization();
    }
    private void maybeForceBuilderInitialization() {
      if (com.google.protobuf.GeneratedMessage.alwaysUseFieldBuilders) {
        getPosCommandFieldBuilder();
        getScriptedCommandFieldBuilder();
      }
    }
    private static Builder create() {
      return new Builder();
    }

    public Builder clear() {
      super.clear();
      type_ = messages.HeadMotionCommand.CommandType.POS_HEAD_COMMAND;
      bitField0_ = (bitField0_ & ~0x00000001);
      if (posCommandBuilder_ == null) {
        posCommand_ = messages.PositionHeadCommand.getDefaultInstance();
      } else {
        posCommandBuilder_.clear();
      }
      bitField0_ = (bitField0_ & ~0x00000002);
      if (scriptedCommandBuilder_ == null) {
        scriptedCommand_ = messages.ScriptedHeadCommand.getDefaultInstance();
      } else {
        scriptedCommandBuilder_.clear();
      }
      bitField0_ = (bitField0_ & ~0x00000004);
      timestamp_ = 0L;
      bitField0_ = (bitField0_ & ~0x00000008);
      return this;
    }

    public Builder clone() {
      return create().mergeFrom(buildPartial());
    }

    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return messages._File_PMotion.internal_static_messages_HeadMotionCommand_descriptor;
    }

    public messages.HeadMotionCommand getDefaultInstanceForType() {
      return messages.HeadMotionCommand.getDefaultInstance();
    }

    public messages.HeadMotionCommand build() {
      messages.HeadMotionCommand result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    public messages.HeadMotionCommand buildPartial() {
      messages.HeadMotionCommand result = new messages.HeadMotionCommand(this);
      int from_bitField0_ = bitField0_;
      int to_bitField0_ = 0;
      if (((from_bitField0_ & 0x00000001) == 0x00000001)) {
        to_bitField0_ |= 0x00000001;
      }
      result.type_ = type_;
      if (((from_bitField0_ & 0x00000002) == 0x00000002)) {
        to_bitField0_ |= 0x00000002;
      }
      if (posCommandBuilder_ == null) {
        result.posCommand_ = posCommand_;
      } else {
        result.posCommand_ = posCommandBuilder_.build();
      }
      if (((from_bitField0_ & 0x00000004) == 0x00000004)) {
        to_bitField0_ |= 0x00000004;
      }
      if (scriptedCommandBuilder_ == null) {
        result.scriptedCommand_ = scriptedCommand_;
      } else {
        result.scriptedCommand_ = scriptedCommandBuilder_.build();
      }
      if (((from_bitField0_ & 0x00000008) == 0x00000008)) {
        to_bitField0_ |= 0x00000008;
      }
      result.timestamp_ = timestamp_;
      result.bitField0_ = to_bitField0_;
      onBuilt();
      return result;
    }

    public Builder mergeFrom(com.google.protobuf.Message other) {
      if (other instanceof messages.HeadMotionCommand) {
        return mergeFrom((messages.HeadMotionCommand)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(messages.HeadMotionCommand other) {
      if (other == messages.HeadMotionCommand.getDefaultInstance()) return this;
      if (other.hasType()) {
        setType(other.getType());
      }
      if (other.hasPosCommand()) {
        mergePosCommand(other.getPosCommand());
      }
      if (other.hasScriptedCommand()) {
        mergeScriptedCommand(other.getScriptedCommand());
      }
      if (other.hasTimestamp()) {
        setTimestamp(other.getTimestamp());
      }
      this.mergeUnknownFields(other.getUnknownFields());
      return this;
    }

    public final boolean isInitialized() {
      return true;
    }

    public Builder mergeFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      messages.HeadMotionCommand parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (messages.HeadMotionCommand) e.getUnfinishedMessage();
        throw e;
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }
    private int bitField0_;

    private messages.HeadMotionCommand.CommandType type_ = messages.HeadMotionCommand.CommandType.POS_HEAD_COMMAND;
    /**
     * <code>optional .messages.HeadMotionCommand.CommandType type = 1;</code>
     */
    public boolean hasType() {
      return ((bitField0_ & 0x00000001) == 0x00000001);
    }
    /**
     * <code>optional .messages.HeadMotionCommand.CommandType type = 1;</code>
     */
    public messages.HeadMotionCommand.CommandType getType() {
      return type_;
    }
    /**
     * <code>optional .messages.HeadMotionCommand.CommandType type = 1;</code>
     */
    public Builder setType(messages.HeadMotionCommand.CommandType value) {
      if (value == null) {
        throw new NullPointerException();
      }
      bitField0_ |= 0x00000001;
      type_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>optional .messages.HeadMotionCommand.CommandType type = 1;</code>
     */
    public Builder clearType() {
      bitField0_ = (bitField0_ & ~0x00000001);
      type_ = messages.HeadMotionCommand.CommandType.POS_HEAD_COMMAND;
      onChanged();
      return this;
    }

    private messages.PositionHeadCommand posCommand_ = messages.PositionHeadCommand.getDefaultInstance();
    private com.google.protobuf.SingleFieldBuilder<
        messages.PositionHeadCommand, messages.PositionHeadCommand.Builder, messages.PositionHeadCommandOrBuilder> posCommandBuilder_;
    /**
     * <code>optional .messages.PositionHeadCommand pos_command = 2;</code>
     */
    public boolean hasPosCommand() {
      return ((bitField0_ & 0x00000002) == 0x00000002);
    }
    /**
     * <code>optional .messages.PositionHeadCommand pos_command = 2;</code>
     */
    public messages.PositionHeadCommand getPosCommand() {
      if (posCommandBuilder_ == null) {
        return posCommand_;
      } else {
        return posCommandBuilder_.getMessage();
      }
    }
    /**
     * <code>optional .messages.PositionHeadCommand pos_command = 2;</code>
     */
    public Builder setPosCommand(messages.PositionHeadCommand value) {
      if (posCommandBuilder_ == null) {
        if (value == null) {
          throw new NullPointerException();
        }
        posCommand_ = value;
        onChanged();
      } else {
        posCommandBuilder_.setMessage(value);
      }
      bitField0_ |= 0x00000002;
      return this;
    }
    /**
     * <code>optional .messages.PositionHeadCommand pos_command = 2;</code>
     */
    public Builder setPosCommand(
        messages.PositionHeadCommand.Builder builderForValue) {
      if (posCommandBuilder_ == null) {
        posCommand_ = builderForValue.build();
        onChanged();
      } else {
        posCommandBuilder_.setMessage(builderForValue.build());
      }
      bitField0_ |= 0x00000002;
      return this;
    }
    /**
     * <code>optional .messages.PositionHeadCommand pos_command = 2;</code>
     */
    public Builder mergePosCommand(messages.PositionHeadCommand value) {
      if (posCommandBuilder_ == null) {
        if (((bitField0_ & 0x00000002) == 0x00000002) &&
            posCommand_ != messages.PositionHeadCommand.getDefaultInstance()) {
          posCommand_ =
            messages.PositionHeadCommand.newBuilder(posCommand_).mergeFrom(value).buildPartial();
        } else {
          posCommand_ = value;
        }
        onChanged();
      } else {
        posCommandBuilder_.mergeFrom(value);
      }
      bitField0_ |= 0x00000002;
      return this;
    }
    /**
     * <code>optional .messages.PositionHeadCommand pos_command = 2;</code>
     */
    public Builder clearPosCommand() {
      if (posCommandBuilder_ == null) {
        posCommand_ = messages.PositionHeadCommand.getDefaultInstance();
        onChanged();
      } else {
        posCommandBuilder_.clear();
      }
      bitField0_ = (bitField0_ & ~0x00000002);
      return this;
    }
    /**
     * <code>optional .messages.PositionHeadCommand pos_command = 2;</code>
     */
    public messages.PositionHeadCommand.Builder getPosCommandBuilder() {
      bitField0_ |= 0x00000002;
      onChanged();
      return getPosCommandFieldBuilder().getBuilder();
    }
    /**
     * <code>optional .messages.PositionHeadCommand pos_command = 2;</code>
     */
    public messages.PositionHeadCommandOrBuilder getPosCommandOrBuilder() {
      if (posCommandBuilder_ != null) {
        return posCommandBuilder_.getMessageOrBuilder();
      } else {
        return posCommand_;
      }
    }
    /**
     * <code>optional .messages.PositionHeadCommand pos_command = 2;</code>
     */
    private com.google.protobuf.SingleFieldBuilder<
        messages.PositionHeadCommand, messages.PositionHeadCommand.Builder, messages.PositionHeadCommandOrBuilder> 
        getPosCommandFieldBuilder() {
      if (posCommandBuilder_ == null) {
        posCommandBuilder_ = new com.google.protobuf.SingleFieldBuilder<
            messages.PositionHeadCommand, messages.PositionHeadCommand.Builder, messages.PositionHeadCommandOrBuilder>(
                getPosCommand(),
                getParentForChildren(),
                isClean());
        posCommand_ = null;
      }
      return posCommandBuilder_;
    }

    private messages.ScriptedHeadCommand scriptedCommand_ = messages.ScriptedHeadCommand.getDefaultInstance();
    private com.google.protobuf.SingleFieldBuilder<
        messages.ScriptedHeadCommand, messages.ScriptedHeadCommand.Builder, messages.ScriptedHeadCommandOrBuilder> scriptedCommandBuilder_;
    /**
     * <code>optional .messages.ScriptedHeadCommand scripted_command = 3;</code>
     */
    public boolean hasScriptedCommand() {
      return ((bitField0_ & 0x00000004) == 0x00000004);
    }
    /**
     * <code>optional .messages.ScriptedHeadCommand scripted_command = 3;</code>
     */
    public messages.ScriptedHeadCommand getScriptedCommand() {
      if (scriptedCommandBuilder_ == null) {
        return scriptedCommand_;
      } else {
        return scriptedCommandBuilder_.getMessage();
      }
    }
    /**
     * <code>optional .messages.ScriptedHeadCommand scripted_command = 3;</code>
     */
    public Builder setScriptedCommand(messages.ScriptedHeadCommand value) {
      if (scriptedCommandBuilder_ == null) {
        if (value == null) {
          throw new NullPointerException();
        }
        scriptedCommand_ = value;
        onChanged();
      } else {
        scriptedCommandBuilder_.setMessage(value);
      }
      bitField0_ |= 0x00000004;
      return this;
    }
    /**
     * <code>optional .messages.ScriptedHeadCommand scripted_command = 3;</code>
     */
    public Builder setScriptedCommand(
        messages.ScriptedHeadCommand.Builder builderForValue) {
      if (scriptedCommandBuilder_ == null) {
        scriptedCommand_ = builderForValue.build();
        onChanged();
      } else {
        scriptedCommandBuilder_.setMessage(builderForValue.build());
      }
      bitField0_ |= 0x00000004;
      return this;
    }
    /**
     * <code>optional .messages.ScriptedHeadCommand scripted_command = 3;</code>
     */
    public Builder mergeScriptedCommand(messages.ScriptedHeadCommand value) {
      if (scriptedCommandBuilder_ == null) {
        if (((bitField0_ & 0x00000004) == 0x00000004) &&
            scriptedCommand_ != messages.ScriptedHeadCommand.getDefaultInstance()) {
          scriptedCommand_ =
            messages.ScriptedHeadCommand.newBuilder(scriptedCommand_).mergeFrom(value).buildPartial();
        } else {
          scriptedCommand_ = value;
        }
        onChanged();
      } else {
        scriptedCommandBuilder_.mergeFrom(value);
      }
      bitField0_ |= 0x00000004;
      return this;
    }
    /**
     * <code>optional .messages.ScriptedHeadCommand scripted_command = 3;</code>
     */
    public Builder clearScriptedCommand() {
      if (scriptedCommandBuilder_ == null) {
        scriptedCommand_ = messages.ScriptedHeadCommand.getDefaultInstance();
        onChanged();
      } else {
        scriptedCommandBuilder_.clear();
      }
      bitField0_ = (bitField0_ & ~0x00000004);
      return this;
    }
    /**
     * <code>optional .messages.ScriptedHeadCommand scripted_command = 3;</code>
     */
    public messages.ScriptedHeadCommand.Builder getScriptedCommandBuilder() {
      bitField0_ |= 0x00000004;
      onChanged();
      return getScriptedCommandFieldBuilder().getBuilder();
    }
    /**
     * <code>optional .messages.ScriptedHeadCommand scripted_command = 3;</code>
     */
    public messages.ScriptedHeadCommandOrBuilder getScriptedCommandOrBuilder() {
      if (scriptedCommandBuilder_ != null) {
        return scriptedCommandBuilder_.getMessageOrBuilder();
      } else {
        return scriptedCommand_;
      }
    }
    /**
     * <code>optional .messages.ScriptedHeadCommand scripted_command = 3;</code>
     */
    private com.google.protobuf.SingleFieldBuilder<
        messages.ScriptedHeadCommand, messages.ScriptedHeadCommand.Builder, messages.ScriptedHeadCommandOrBuilder> 
        getScriptedCommandFieldBuilder() {
      if (scriptedCommandBuilder_ == null) {
        scriptedCommandBuilder_ = new com.google.protobuf.SingleFieldBuilder<
            messages.ScriptedHeadCommand, messages.ScriptedHeadCommand.Builder, messages.ScriptedHeadCommandOrBuilder>(
                getScriptedCommand(),
                getParentForChildren(),
                isClean());
        scriptedCommand_ = null;
      }
      return scriptedCommandBuilder_;
    }

    private long timestamp_ ;
    /**
     * <code>optional int64 timestamp = 4;</code>
     */
    public boolean hasTimestamp() {
      return ((bitField0_ & 0x00000008) == 0x00000008);
    }
    /**
     * <code>optional int64 timestamp = 4;</code>
     */
    public long getTimestamp() {
      return timestamp_;
    }
    /**
     * <code>optional int64 timestamp = 4;</code>
     */
    public Builder setTimestamp(long value) {
      bitField0_ |= 0x00000008;
      timestamp_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>optional int64 timestamp = 4;</code>
     */
    public Builder clearTimestamp() {
      bitField0_ = (bitField0_ & ~0x00000008);
      timestamp_ = 0L;
      onChanged();
      return this;
    }

    // @@protoc_insertion_point(builder_scope:messages.HeadMotionCommand)
  }

  static {
    defaultInstance = new HeadMotionCommand(true);
    defaultInstance.initFields();
  }

  // @@protoc_insertion_point(class_scope:messages.HeadMotionCommand)
}

