// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: InertialState.proto

package messages;

public interface InertialStateOrBuilder extends
    // @@protoc_insertion_point(interface_extends:messages.InertialState)
    com.google.protobuf.MessageOrBuilder {

  /**
   * <code>optional float acc_x = 1;</code>
   *
   * <pre>
   * Raw accelerometer data.
   * </pre>
   */
  boolean hasAccX();
  /**
   * <code>optional float acc_x = 1;</code>
   *
   * <pre>
   * Raw accelerometer data.
   * </pre>
   */
  float getAccX();

  /**
   * <code>optional float acc_y = 2;</code>
   */
  boolean hasAccY();
  /**
   * <code>optional float acc_y = 2;</code>
   */
  float getAccY();

  /**
   * <code>optional float acc_z = 3;</code>
   */
  boolean hasAccZ();
  /**
   * <code>optional float acc_z = 3;</code>
   */
  float getAccZ();

  /**
   * <code>optional float gyr_x = 4;</code>
   *
   * <pre>
   * Raw gyrometer data.
   * </pre>
   */
  boolean hasGyrX();
  /**
   * <code>optional float gyr_x = 4;</code>
   *
   * <pre>
   * Raw gyrometer data.
   * </pre>
   */
  float getGyrX();

  /**
   * <code>optional float gyr_y = 5;</code>
   */
  boolean hasGyrY();
  /**
   * <code>optional float gyr_y = 5;</code>
   */
  float getGyrY();

  /**
   * <code>optional float gyr_z = 6;</code>
   */
  boolean hasGyrZ();
  /**
   * <code>optional float gyr_z = 6;</code>
   */
  float getGyrZ();

  /**
   * <code>optional float angle_x = 7;</code>
   *
   * <pre>
   * Filtered angle data.
   * </pre>
   */
  boolean hasAngleX();
  /**
   * <code>optional float angle_x = 7;</code>
   *
   * <pre>
   * Filtered angle data.
   * </pre>
   */
  float getAngleX();

  /**
   * <code>optional float angle_y = 8;</code>
   */
  boolean hasAngleY();
  /**
   * <code>optional float angle_y = 8;</code>
   */
  float getAngleY();

  /**
   * <code>optional float angle_z = 9;</code>
   */
  boolean hasAngleZ();
  /**
   * <code>optional float angle_z = 9;</code>
   */
  float getAngleZ();
}
