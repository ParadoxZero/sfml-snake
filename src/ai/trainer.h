#pragma once
#include <SFML/Graphics.hpp>
#include "core/game.h"
#include "ai/dqn.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

namespace ai_trainer {

inline void draw_ui(sf::RenderWindow& window, sf::Font& font,
                    int ep, int max_ep, float avg_score, float epsilon) {
    window.clear(sf::Color(20, 20, 30));

    sf::Text title("AI Training in Progress", font, 32);
    title.setFillColor(sf::Color(100, 220, 100));
    title.setPosition(80, 60);
    window.draw(title);

    float bar_w = 640.f;
    sf::RectangleShape bar_bg(sf::Vector2f(bar_w, 24));
    bar_bg.setPosition(80, 130);
    bar_bg.setFillColor(sf::Color(60, 60, 80));
    window.draw(bar_bg);

    sf::RectangleShape bar_fill(sf::Vector2f(bar_w * ep / max_ep, 24));
    bar_fill.setPosition(80, 130);
    bar_fill.setFillColor(sf::Color(80, 160, 255));
    window.draw(bar_fill);

    auto draw_text = [&](const std::string& s, float x, float y,
                         sf::Color c = sf::Color::White) {
        sf::Text t(s, font, 24);
        t.setFillColor(c);
        t.setPosition(x, y);
        window.draw(t);
    };

    std::ostringstream oss;
    oss << "Episode:   " << ep << " / " << max_ep;
    draw_text(oss.str(), 80, 180);

    oss.str("");
    oss << std::fixed << std::setprecision(2)
        << "Avg score: " << avg_score << "  (last 50 episodes)";
    draw_text(oss.str(), 80, 220);

    oss.str("");
    oss << std::fixed << std::setprecision(3) << "Epsilon:   " << epsilon;
    draw_text(oss.str(), 80, 260,
              sf::Color(epsilon > 0.5f ? 255 : 100, 200, 100));

    draw_text("Close window to stop training.", 80, 740, sf::Color(150, 150, 150));
    window.display();
}

inline void run_training(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("sansation.ttf");

    game::GameController env(&window);
    dqn::DQNAgent agent(8, game::ACTION_COUNT);

    constexpr int MAX_EPISODES   = 5000;
    constexpr int MAX_STEPS      = 1000;
    constexpr int AVG_WINDOW     = 50;   // rolling average window
    constexpr int LOG_EVERY      = 50;   // console log frequency

    std::vector<int> scores;
    float avg_score = 0.0f;

    draw_ui(window, font, 0, MAX_EPISODES, 0.0f, agent.epsilon);

    for (int ep = 0; ep < MAX_EPISODES; ep++) {
        env.reset();
        auto state = dqn::normalize_state(env.AI_GetState());

        int  food_count = 0;
        bool done       = false;

        for (int step = 0; step < MAX_STEPS && !done; step++) {
            sf::Event event;
            while (window.pollEvent(event))
                if (event.type == sf::Event::Closed) return;

            int action = agent.select_action(state);
            auto [raw_next, reward, next_done] = env.AI_HeadlessStep(action);

            auto next_state = dqn::normalize_state(raw_next);
            agent.store(state, action, reward, next_state, next_done);
            agent.train_step();

            if (reward >= 10.0f) food_count++;
            state = next_state;
            done  = next_done;
        }

        scores.push_back(food_count);

        // Recompute rolling average
        int n = std::min(AVG_WINDOW, (int)scores.size());
        avg_score = 0.0f;
        for (int i = (int)scores.size() - n; i < (int)scores.size(); i++)
            avg_score += scores[i];
        avg_score /= n;

        // Update UI every episode so progress bar always moves
        draw_ui(window, font, ep + 1, MAX_EPISODES, avg_score, agent.epsilon);

        if ((ep + 1) % LOG_EVERY == 0) {
            std::cout << "Episode " << std::setw(5) << ep + 1
                      << "  |  Avg score: " << std::fixed << std::setprecision(2) << avg_score
                      << "  |  epsilon: "   << std::setprecision(3) << agent.epsilon << "\n";
        }
    }

    agent.save("snake_model.bin");
    std::cout << "Training done. Model saved to snake_model.bin\n";
}

// ─── AI Play ──────────────────────────────────────────────────────────────────

inline void draw_not_trained(sf::RenderWindow& window, sf::Font& font) {
    window.clear(sf::Color(20, 20, 30));
    sf::Text msg("Model not trained yet.\nPlease run AI Training first.\n\nPress any key to return.", font, 28);
    msg.setFillColor(sf::Color(220, 80, 80));
    msg.setPosition(80, 300);
    window.draw(msg);
    window.display();
}

inline void run_ai_play(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("sansation.ttf");

    dqn::DQNAgent agent(8, game::ACTION_COUNT);
    if (!agent.load("snake_model.bin")) {
        draw_not_trained(window, font);
        sf::Event event;
        while (window.waitEvent(event)) {
            if (event.type == sf::Event::Closed ||
                event.type == sf::Event::KeyPressed) break;
        }
        return;
    }

    game::GameController env(&window);
    env.AI_GameLoop([&](game::State s) {
        auto state = dqn::normalize_state(s);
        auto q     = agent.policy_net.predict(state);
        return (int)(std::max_element(q.begin(), q.end()) - q.begin());
    });
}

} // namespace ai_trainer
