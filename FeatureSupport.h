/**
 * FeatureSupport Class
 *
 * A base class that provides runtime feature management capabilities.
 * Classes can inherit from this class to add and check features at runtime.
 *
 * Example usage:
 *
 * class MyComponent : public FeatureSupport {
 *   public:
 *     MyComponent() {
 *       addSupportFeature("distance", true);
 *       addSupportFeature("motor", false);
 *     }
 *
 *     void doSomething() {
 *       if (hasFeature("distance")) {
 *         // Do something with distance sensor
 *       }
 *     }
 * };
 */
class FeatureSupport {
private:
  std::map<String, bool> _features;

public:
  /**
   * Check if a feature is supported
   *
   * @param key The feature key to check
   * @return true if the feature is supported, false otherwise
   */
  bool hasFeature(String key) {
    auto it = _features.find(key);
    return (it != _features.end()) ? it->second : false;
  }

private:
  /**
   * Add or update a feature
   *
   * @param key The feature key
   * @param value Whether the feature is supported (true) or not (false)
   */
  void addSupportFeature(String key, bool value) {
    _features[key] = value;
  }
};