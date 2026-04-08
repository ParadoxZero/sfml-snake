#pragma once
#include <SFML/Graphics.hpp>
#include "core/game.h"
#include "ai/dqn.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstdio>

namespace ai_trainer {

// ─── UI helpers ───────────────────────────────────────────────────────────────

static const sf::FloatRect RESET_BTN(560, 700, 160, 44);

inline void draw_ui(sf::RenderWindow& window, sf::Font& font,
                    int ep, int max_ep, float avg_score, float epsilon) {
    window.clear(sf::Color(20, 20, 30));

    // Title
    sf::Text title("AI Training in Progress", font, 32);
    title.setFillColor(sf::Color(100, 220, 100));
    title.setPosition(80, 60);
    window.draw(title);

    // Progress bar background
    sf::RectangleShape bar_bg(sf::Vector2f(640.f, 24));
    bar_bg.setPosition(80, 130);
    bar_bg.setFillColor(sf::Color(60, 60, 80));
    window.draw(bar_bg);

    // Progress bar fill
    float fill = (max_ep > 0) ? 640.f * ep / max_ep : 0.f;
    sf::RectangleShape bar_fill(sf::Vector2f(fill, 24));
    bar_fill.setPosition(80, 130);
    bar_fill.setFillColor(sf::Color(80, 160, 255));
    window.draw(bar_fill);

    // Stats
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

    draw_text("Press ESC to pause & return to menu.", 80, 750, sf::Color(150, 150, 150));

    // Reset button
    sf::RectangleShape btn(sf::Vector2f(RESET_BTN.width, RESET_BTN.height));
    btn.setPosition(RESET_BTN.left, RESET_BTN.top);
    btn.setFillColor(sf::Color(180, 50, 50));
    btn.setOutlineColor(sf::Color(240, 80, 80));
    btn.setOutlineThickness(2);
    window.draw(btn);

    sf::Text btn_label("Reset Training", font, 20);
    btn_label.setFillColor(sf::Color::White);
    btn_label.setPosition(RESET_BTN.left + 8, RESET_BTN.top + 10);
    window.draw(btn_label);

    window.display();
}

// ─── Training loop ────────────────────────────────────────────────────────────

inline void run_training(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("sansation.ttf");

    constexpr int MAX_EPISODES     = 5000;
    constexpr int MAX_STEPS        = 1000;
    constexpr int AVG_WINDOW       = 50;
    constexpr int LOG_EVERY        = 50;
    constexpr int CHECKPOINT_EVERY = 100;
    const std::string CKPT_PATH    = "snake_checkpoint.bin";
    const std::string MODEL_PATH   = "snake_model.bin";

    // Lambda to initialise a fresh agent + episode counter
    auto make_agent = []() {
        return dqn::DQNAgent(8, game::ACTION_COUNT);
    };

    dqn::DQNAgent agent = make_agent();
    int start_ep = 0;

    if (agent.load_checkpoint(CKPT_PATH, start_ep)) {
        std::cout << "Resumed from checkpoint at episode " << start_ep << "\n";
    } else {
        std::cout << "No checkpoint found, starting fresh.\n";
    }

    std::vector<int> scores;
    float avg_score = 0.0f;

    draw_ui(window, font, start_ep, MAX_EPISODES, 0.0f, agent.epsilon);

    game::GameController env(&window);

    int ep = start_ep;
    while (ep < MAX_EPISODES) {
        env.reset();
        auto state = dqn::normalize_state(env.AI_GetState());

        int  food_count = 0;
        bool done       = false;
        bool user_quit  = false;
        bool user_reset = false;

        for (int step = 0; step < MAX_STEPS && !done; step++) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    user_quit = true;
                }
                if (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Escape) {
                    user_quit = true;
                }
                if (event.type == sf::Event::MouseButtonPressed &&
                    event.mouseButton.button == sf::Mouse::Left) {
                    float mx = (float)event.mouseButton.x;
                    float my = (float)event.mouseButton.y;
                    if (RESET_BTN.contains(mx, my)) {
                        user_reset = true;
                    }
                }
            }
            if (user_quit || user_reset) break;

            int action = agent.select_action(state);
            auto [raw_next, reward, next_done] = env.AI_HeadlessStep(action);

            auto next_state = dqn::normalize_state(raw_next);
            agent.store(state, action, reward, next_state, next_done);
            agent.train_step();

            if (reward >= 10.0f) food_count++;
            state = next_state;
            done  = next_done;
        }

        // ── Handle reset ──────────────────────────────────────────────────────
        if (user_reset) {
            std::remove(CKPT_PATH.c_str());
            std::remove(MODEL_PATH.c_str());
            agent    = make_agent();
            start_ep = 0;
            ep       = 0;
            scores.clear();
            avg_score = 0.0f;
            std::cout << "Training reset.\n";
            draw_ui(window, font, 0, MAX_EPISODES, 0.0f, agent.epsilon);
            continue;
        }

        // ── Handle ESC / window close — save current ep (not ep+1) ───────────
        if (user_quit) {
            agent.save_checkpoint(CKPT_PATH, ep); // ep not yet completed → resume from ep
            agent.save(MODEL_PATH);
            std::cout << "Paused. Checkpoint saved at episode " << ep << "\n";
            return;
        }

        scores.push_back(food_count);

        // Rolling average
        int n = std::min(AVG_WINDOW, (int)scores.size());
        avg_score = 0.0f;
        for (int i = (int)scores.size() - n; i < (int)scores.size(); i++)
            avg_score += scores[i];
        avg_score /= n;

        ep++;
        draw_ui(window, font, ep, MAX_EPISODES, avg_score, agent.epsilon);

        if (ep % LOG_EVERY == 0) {
            std::cout << "Episode " << std::setw(5) << ep
                      << "  |  Avg score: " << std::fixed << std::setprecision(2) << avg_score
                      << "  |  epsilon: "   << std::setprecision(3) << agent.epsilon << "\n";
        }

        if (ep % CHECKPOINT_EVERY == 0) {
            agent.save_checkpoint(CKPT_PATH, ep);
            std::cout << "Checkpoint saved at episode " << ep << "\n";
        }
    }

    // Training complete
    agent.save_checkpoint(CKPT_PATH, MAX_EPISODES);
    agent.save(MODEL_PATH);
    std::cout << "Training done. Model saved to " << MODEL_PATH << "\n";
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
