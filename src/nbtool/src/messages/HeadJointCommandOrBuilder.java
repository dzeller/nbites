// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: PMotion.proto

package messages;

public interface HeadJointCommandOrBuilder extends
    // @@protoc_insertion_point(interface_extends:messages.HeadJointCommand)
    com.google.protobuf.MessageOrBuilder {

  /**
   * <code>optional float time = 1;</code>
   */
  boolean hasTime();
  /**
   * <code>optional float time = 1;</code>
   */
  float getTime();

  /**
   * <code>optional .messages.JointAngles angles = 2;</code>
   */
  boolean hasAngles();
  /**
   * <code>optional .messages.JointAngles angles = 2;</code>
   */
  messages.JointAngles getAngles();
  /**
   * <code>optional .messages.JointAngles angles = 2;</code>
   */
  messages.JointAnglesOrBuilder getAnglesOrBuilder();

  /**
   * <code>optional .messages.JointAngles stiffness = 3;</code>
   */
  boolean hasStiffness();
  /**
   * <code>optional .messages.JointAngles stiffness = 3;</code>
   */
  messages.JointAngles getStiffness();
  /**
   * <code>optional .messages.JointAngles stiffness = 3;</code>
   */
  messages.JointAnglesOrBuilder getStiffnessOrBuilder();

  /**
   * <code>optional .messages.HeadJointCommand.InterpolationType interpolation = 4;</code>
   */
  boolean hasInterpolation();
  /**
   * <code>optional .messages.HeadJointCommand.InterpolationType interpolation = 4;</code>
   */
  messages.HeadJointCommand.InterpolationType getInterpolation();
}
