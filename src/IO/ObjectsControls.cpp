#include "include/IO/ObjectsControls.hpp"
#include "IO/KeysControls.hpp"

void ObjectsControls::initControls() {
    using BehaviourCtorTag = std::variant< std::in_place_type_t<Behaviours::Spin>, std::in_place_type_t<Behaviours::Translation> >; 
    static BehaviourCtorTag selectedTag;
    selectedTag = std::in_place_type<Behaviours::Translation>;

    auto& eventChannel = EventChannel::getInstance();

    eventChannel.publish("NewKeyReleaseEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_1, [this](Window *) {
      selectedTag = std::in_place_type<Behaviours::Spin>;
    }));

    eventChannel.publish("NewKeyReleaseEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_2, [this](Window *) {
      selectedTag = std::in_place_type<Behaviours::Translation>;
    }));

    eventChannel.publish("NewKeyReleaseEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_0, [this](Window *) {
      if (!m_objects3d.empty())
        m_behaviours.removeBehaviourFromObject(*m_selectedObjectIt);
    }));

    eventChannel.publish("NewKeyReleaseEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_LEFT, [this](Window *) {
      if (m_objects3d.empty())
        return;
      if (m_selectedObjectIt == m_objects3d.begin()) {
        m_selectedObjectIt = m_objects3d.end() - 1;
      } else {
        m_selectedObjectIt--;
      }
    }));

    eventChannel.publish("NewKeyReleaseEvent", std::make_pair<int, std::function<void(Window * window)>>(GLFW_KEY_RIGHT, [this](Window *) {
      if (m_objects3d.empty())
        return;
      m_selectedObjectIt++;
      if (m_selectedObjectIt == m_objects3d.end())
        m_selectedObjectIt = m_objects3d.begin();
    }));

    auto addKeyBinding = [ this ]( int key, glm42::vec3 delta ) {
      auto addBehaviour = [ this ]( const glm42::vec3& delta ) {
        std::visit( [ this, delta ]( auto tag ) {
              if (m_objects3d.empty())
                return;

              TransformationBehaviours::BehaviourVariant v{ tag, delta };
              m_behaviours.addBehaviourToObject( *m_selectedObjectIt, std::move( v ) );
            }, selectedTag );
      };

      return std::make_pair< int, std::function< void( Window * window ) > >(
          std::move( key ), [ addBehaviour, delta ]( Window* ) { addBehaviour( delta ); } );
    };

    // X
    eventChannel.publish("NewKeyReleaseEvent", addKeyBinding(GLFW_KEY_U, glm42::vec3{0.05f, 0.0f, 0.0f}));
    eventChannel.publish("NewKeyReleaseEvent", addKeyBinding(GLFW_KEY_J, glm42::vec3{-0.05f, 0.0f, 0.0f}));

    // Y
    eventChannel.publish("NewKeyReleaseEvent", addKeyBinding(GLFW_KEY_I, glm42::vec3{0.0f, 0.05f, 0.0f}));
    eventChannel.publish("NewKeyReleaseEvent", addKeyBinding(GLFW_KEY_K, glm42::vec3{0.0f, -0.05f, 0.0f}));

    // Z
    eventChannel.publish("NewKeyReleaseEvent", addKeyBinding(GLFW_KEY_O, glm42::vec3{0.0f, 0.0f, 0.05f}));
    eventChannel.publish("NewKeyReleaseEvent", addKeyBinding(GLFW_KEY_L, glm42::vec3{0.0f, 0.0f, -0.05f}));
  }
