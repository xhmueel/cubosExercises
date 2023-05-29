/// @file
/// @brief Contains the Input class.

#pragma once

#include <cubos/core/io/window.hpp>

#include <cubos/engine/input/bindings.hpp>

namespace cubos::engine
{
    /// Used to store the input bindings for multiple players and update its state accordingly as events are received.
    class Input final
    {
    public:
        using Key = core::io::Key;
        using Modifiers = core::io::Modifiers;

        Input() = default;
        ~Input() = default;

        /// Clears all bindings.
        void clear();

        /// Clears all bindings for a specific player.
        /// @param player The player whose bindings will be cleared.
        void clear(int player);

        /// Sets the bindings for a specific player.
        /// @param bindings The bindings to set.
        /// @param player The player whose bindings will be set.
        void bind(const InputBindings& bindings, int player = 0);

        /// Gets an action state for a specific player.
        /// @param actionName The name of the action.
        /// @param player The player whose action state will be retrieved.
        /// @return True if the action exists and is pressed, false otherwise.
        bool pressed(const char* actionName, int player = 0) const;

        /// Gets a key state.
        /// @param Key The key.
        /// @param modifiers The modifiers (optional).
        /// @return True if the key is pressed, false otherwise.
        bool pressed(Key key, Modifiers modifiers = Modifiers::None) const;

        /// Gets an axis value for a specific player.
        /// @param axisName The name of the axis.
        /// @param player The player whose axis value will be retrieved.
        /// @return The axis value if the axis exists, 0.0 otherwise.
        float axis(const char* axisName, int player = 0) const;

        /// Handle a key event.
        /// @param event The key event.
        void handle(const core::io::KeyEvent& event);

        /// Handle a modifiers event.
        /// @param event The modifiers event.
        void handle(const core::io::ModifiersEvent& event);

        /// Discard remaining events.
        /// @param event The event to discard.
        inline void handle(const core::io::WindowEvent& event)
        {
            (void)event;
        }

        /// @return The bindings.
        const std::unordered_map<int, InputBindings>& bindings() const;

    private:
        struct BindingIndex
        {
            std::string name;      ///< The name of the action or axis.
            int player;            ///< The player index.
            bool negative = false; ///< Whether the pressed key is a negative axis key.
        };

        bool anyPressed(const std::vector<std::pair<Key, Modifiers>>& keys) const;

        std::unordered_map<int, InputBindings> mBindings;

        Modifiers mModifiers = Modifiers::None;
        std::unordered_map<Key, bool> mPressedKeys;

        std::unordered_map<Key, std::vector<BindingIndex>> mBoundActions;
        std::unordered_map<Key, std::vector<BindingIndex>> mBoundAxes;
    };
} // namespace cubos::engine