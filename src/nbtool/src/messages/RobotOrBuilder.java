// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: VisionRobot.proto

package messages;

public interface RobotOrBuilder extends
    // @@protoc_insertion_point(interface_extends:messages.Robot)
    com.google.protobuf.MessageOrBuilder {

  /**
   * <code>optional float distance = 1;</code>
   */
  boolean hasDistance();
  /**
   * <code>optional float distance = 1;</code>
   */
  float getDistance();

  /**
   * <code>optional float bearing = 2;</code>
   */
  boolean hasBearing();
  /**
   * <code>optional float bearing = 2;</code>
   */
  float getBearing();

  /**
   * <code>optional float bearing_deg = 3;</code>
   */
  boolean hasBearingDeg();
  /**
   * <code>optional float bearing_deg = 3;</code>
   */
  float getBearingDeg();

  /**
   * <code>optional float angle_x_deg = 4;</code>
   */
  boolean hasAngleXDeg();
  /**
   * <code>optional float angle_x_deg = 4;</code>
   */
  float getAngleXDeg();

  /**
   * <code>optional float angle_y_deg = 5;</code>
   */
  boolean hasAngleYDeg();
  /**
   * <code>optional float angle_y_deg = 5;</code>
   */
  float getAngleYDeg();

  /**
   * <code>optional sint32 x = 6;</code>
   */
  boolean hasX();
  /**
   * <code>optional sint32 x = 6;</code>
   */
  int getX();

  /**
   * <code>optional sint32 y = 7;</code>
   */
  boolean hasY();
  /**
   * <code>optional sint32 y = 7;</code>
   */
  int getY();

  /**
   * <code>optional float elevation_deg = 8;</code>
   */
  boolean hasElevationDeg();
  /**
   * <code>optional float elevation_deg = 8;</code>
   */
  float getElevationDeg();

  /**
   * <code>optional bool on = 9;</code>
   */
  boolean hasOn();
  /**
   * <code>optional bool on = 9;</code>
   */
  boolean getOn();

  /**
   * <code>optional sint32 height = 10;</code>
   */
  boolean hasHeight();
  /**
   * <code>optional sint32 height = 10;</code>
   */
  int getHeight();

  /**
   * <code>optional sint32 width = 11;</code>
   */
  boolean hasWidth();
  /**
   * <code>optional sint32 width = 11;</code>
   */
  int getWidth();
}