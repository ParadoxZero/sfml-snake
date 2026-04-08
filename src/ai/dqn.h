#pragma once
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cassert>
#include <deque>
#include <array>
#include <fstream>
#include <string>

// Self-contained DQN implementation. No game dependencies.
namespace dqn {

// ─── Matrix ───────────────────────────────────────────────────────────────────

struct Matrix {
    int rows, cols;
    std::vector<float> data;
    Matrix() : rows(0), cols(0) {}
    Matrix(int r, int c, float init = 0.0f)
        : rows(r), cols(c), data(r * c, init) {}
    float&       at(int r, int c)       { return data[r * cols + c]; }
    float        at(int r, int c) const { return data[r * cols + c]; }
};

inline Matrix mat_mul(const Matrix& A, const Matrix& B) {
    assert(A.cols == B.rows);
    Matrix C(A.rows, B.cols, 0.0f);
    for (int i = 0; i < A.rows; i++)
        for (int k = 0; k < A.cols; k++)
            for (int j = 0; j < B.cols; j++)
                C.at(i,j) += A.at(i,k) * B.at(k,j);
    return C;
}

inline Matrix transpose(const Matrix& A) {
    Matrix T(A.cols, A.rows);
    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < A.cols; j++)
            T.at(j,i) = A.at(i,j);
    return T;
}

inline void add_bias(Matrix& m, const std::vector<float>& b) {
    for (int i = 0; i < m.rows; i++)
        for (int j = 0; j < m.cols; j++)
            m.at(i,j) += b[j];
}

inline Matrix relu(const Matrix& m) {
    Matrix out = m;
    for (auto& v : out.data) v = std::max(0.0f, v);
    return out;
}

// ReLU derivative: 1 where pre-activation > 0
inline Matrix relu_mask(const Matrix& pre) {
    Matrix mask(pre.rows, pre.cols);
    for (int i = 0; i < (int)pre.data.size(); i++)
        mask.data[i] = pre.data[i] > 0.0f ? 1.0f : 0.0f;
    return mask;
}

inline Matrix elem_mul(const Matrix& A, const Matrix& B) {
    assert(A.rows == B.rows && A.cols == B.cols);
    Matrix C(A.rows, A.cols);
    for (int i = 0; i < (int)A.data.size(); i++)
        C.data[i] = A.data[i] * B.data[i];
    return C;
}

// ─── Neural Network (MLP: in → h1 → h2 → out, ReLU hidden, linear output) ───

struct NeuralNetwork {
    int in_size, h1_size, h2_size, out_size;
    float lr;

    Matrix W1, W2, W3;
    std::vector<float> b1, b2, b3;

    // Adam state
    Matrix mW1, vW1, mW2, vW2, mW3, vW3;
    std::vector<float> mb1, vb1, mb2, vb2, mb3, vb3;
    int t = 0;
    static constexpr float beta1 = 0.9f, beta2 = 0.999f, eps_adam = 1e-8f;

    NeuralNetwork(int in, int h1, int h2, int out, float learning_rate = 0.001f)
        : in_size(in), h1_size(h1), h2_size(h2), out_size(out), lr(learning_rate),
          W1(h1,in), W2(h2,h1), W3(out,h2),
          b1(h1,0), b2(h2,0), b3(out,0),
          mW1(h1,in,0), vW1(h1,in,0),
          mW2(h2,h1,0), vW2(h2,h1,0),
          mW3(out,h2,0), vW3(out,h2,0),
          mb1(h1,0), vb1(h1,0),
          mb2(h2,0), vb2(h2,0),
          mb3(out,0), vb3(out,0)
    {
        // He initialization
        std::mt19937 rng(42);
        auto he = [&](Matrix& W, int fan_in) {
            float s = std::sqrt(2.0f / fan_in);
            std::normal_distribution<float> d(0.0f, s);
            for (auto& v : W.data) v = d(rng);
        };
        he(W1, in); he(W2, h1); he(W3, h2);
    }

    struct Cache {
        Matrix h1_pre, h1, h2_pre, h2, out;
    };

    Cache forward(const Matrix& x) const {
        Cache c;
        c.h1_pre = mat_mul(x, transpose(W1)); add_bias(c.h1_pre, b1);
        c.h1     = relu(c.h1_pre);
        c.h2_pre = mat_mul(c.h1, transpose(W2)); add_bias(c.h2_pre, b2);
        c.h2     = relu(c.h2_pre);
        c.out    = mat_mul(c.h2, transpose(W3)); add_bias(c.out, b3);
        return c;
    }

    std::vector<float> predict(const std::vector<float>& state) const {
        Matrix x(1, in_size);
        x.data = state;
        auto c = forward(x);
        return c.out.data;
    }

    // Train: MSE loss on selected action only
    void train_batch(const Matrix& x,
                     const std::vector<int>& actions,
                     const std::vector<float>& targets) {
        int B = x.rows;
        auto c = forward(x);

        // dL/d_out — only selected actions get gradient
        Matrix d_out(B, out_size, 0.0f);
        for (int i = 0; i < B; i++) {
            float err = c.out.at(i, actions[i]) - targets[i];
            d_out.at(i, actions[i]) = 2.0f * err / B;
        }

        // Layer 3
        Matrix dW3 = mat_mul(transpose(d_out), c.h2);
        std::vector<float> db3(out_size, 0.0f);
        for (int i = 0; i < B; i++)
            for (int j = 0; j < out_size; j++)
                db3[j] += d_out.at(i,j);

        // Layer 2
        Matrix d_h2     = mat_mul(d_out, W3);
        Matrix d_h2_pre = elem_mul(d_h2, relu_mask(c.h2_pre));
        Matrix dW2      = mat_mul(transpose(d_h2_pre), c.h1);
        std::vector<float> db2(h2_size, 0.0f);
        for (int i = 0; i < B; i++)
            for (int j = 0; j < h2_size; j++)
                db2[j] += d_h2_pre.at(i,j);

        // Layer 1
        Matrix d_h1     = mat_mul(d_h2_pre, W2);
        Matrix d_h1_pre = elem_mul(d_h1, relu_mask(c.h1_pre));
        Matrix dW1      = mat_mul(transpose(d_h1_pre), x);
        std::vector<float> db1(h1_size, 0.0f);
        for (int i = 0; i < B; i++)
            for (int j = 0; j < h1_size; j++)
                db1[j] += d_h1_pre.at(i,j);

        // Adam update
        t++;
        float bc1 = 1.0f - std::pow(beta1, t);
        float bc2 = 1.0f - std::pow(beta2, t);
        adam(W1, mW1, vW1, dW1, bc1, bc2);
        adam(W2, mW2, vW2, dW2, bc1, bc2);
        adam(W3, mW3, vW3, dW3, bc1, bc2);
        adam_vec(b1, mb1, vb1, db1, bc1, bc2);
        adam_vec(b2, mb2, vb2, db2, bc1, bc2);
        adam_vec(b3, mb3, vb3, db3, bc1, bc2);
    }

    void copy_weights_from(const NeuralNetwork& src) {
        W1=src.W1; W2=src.W2; W3=src.W3;
        b1=src.b1; b2=src.b2; b3=src.b3;
    }

    // Save weights only (for inference)
    void save(const std::string& path) const {
        std::ofstream f(path, std::ios::binary);
        auto wm = [&](const Matrix& m) {
            f.write(reinterpret_cast<const char*>(m.data.data()),
                    (std::streamsize)(m.data.size() * sizeof(float)));
        };
        auto wv = [&](const std::vector<float>& v) {
            f.write(reinterpret_cast<const char*>(v.data()),
                    (std::streamsize)(v.size() * sizeof(float)));
        };
        wm(W1); wv(b1); wm(W2); wv(b2); wm(W3); wv(b3);
    }

    bool load(const std::string& path) {
        std::ifstream f(path, std::ios::binary);
        if (!f) return false;
        auto rm = [&](Matrix& m) {
            f.read(reinterpret_cast<char*>(m.data.data()),
                   (std::streamsize)(m.data.size() * sizeof(float)));
        };
        auto rv = [&](std::vector<float>& v) {
            f.read(reinterpret_cast<char*>(v.data()),
                   (std::streamsize)(v.size() * sizeof(float)));
        };
        rm(W1); rv(b1); rm(W2); rv(b2); rm(W3); rv(b3);
        return f.good();
    }

    // Save/load weights + Adam state (for resuming training)
    void write_full(std::ofstream& f) const {
        auto wm = [&](const Matrix& m) {
            f.write(reinterpret_cast<const char*>(m.data.data()),
                    (std::streamsize)(m.data.size() * sizeof(float)));
        };
        auto wv = [&](const std::vector<float>& v) {
            f.write(reinterpret_cast<const char*>(v.data()),
                    (std::streamsize)(v.size() * sizeof(float)));
        };
        wm(W1); wv(b1); wm(W2); wv(b2); wm(W3); wv(b3);
        // Adam state
        wm(mW1); wm(vW1); wm(mW2); wm(vW2); wm(mW3); wm(vW3);
        wv(mb1); wv(vb1); wv(mb2); wv(vb2); wv(mb3); wv(vb3);
        f.write(reinterpret_cast<const char*>(&t), sizeof(t));
    }

    void read_full(std::ifstream& f) {
        auto rm = [&](Matrix& m) {
            f.read(reinterpret_cast<char*>(m.data.data()),
                   (std::streamsize)(m.data.size() * sizeof(float)));
        };
        auto rv = [&](std::vector<float>& v) {
            f.read(reinterpret_cast<char*>(v.data()),
                   (std::streamsize)(v.size() * sizeof(float)));
        };
        rm(W1); rv(b1); rm(W2); rv(b2); rm(W3); rv(b3);
        rm(mW1); rm(vW1); rm(mW2); rm(vW2); rm(mW3); rm(vW3);
        rv(mb1); rv(vb1); rv(mb2); rv(vb2); rv(mb3); rv(vb3);
        f.read(reinterpret_cast<char*>(&t), sizeof(t));
    }

private:
    void adam(Matrix& W, Matrix& m, Matrix& v, const Matrix& g, float bc1, float bc2) {
        for (int i = 0; i < (int)W.data.size(); i++) {
            m.data[i] = beta1*m.data[i] + (1-beta1)*g.data[i];
            v.data[i] = beta2*v.data[i] + (1-beta2)*g.data[i]*g.data[i];
            W.data[i] -= lr * (m.data[i]/bc1) / (std::sqrt(v.data[i]/bc2) + eps_adam);
        }
    }
    void adam_vec(std::vector<float>& w, std::vector<float>& m, std::vector<float>& v,
                  const std::vector<float>& g, float bc1, float bc2) {
        for (int i = 0; i < (int)w.size(); i++) {
            m[i] = beta1*m[i] + (1-beta1)*g[i];
            v[i] = beta2*v[i] + (1-beta2)*g[i]*g[i];
            w[i] -= lr * (m[i]/bc1) / (std::sqrt(v[i]/bc2) + eps_adam);
        }
    }
};

// ─── Replay Buffer ────────────────────────────────────────────────────────────

struct Experience {
    std::vector<float> state, next_state;
    int   action;
    float reward;
    bool  done;
};

class ReplayBuffer {
public:
    explicit ReplayBuffer(int cap) : capacity(cap), rng(123) {}

    void push(Experience e) {
        if ((int)buf.size() >= capacity) buf.pop_front();
        buf.push_back(std::move(e));
    }

    std::vector<Experience> sample(int n) {
        std::vector<int> idx(buf.size());
        std::iota(idx.begin(), idx.end(), 0);
        std::shuffle(idx.begin(), idx.end(), rng);
        std::vector<Experience> batch;
        batch.reserve(n);
        for (int i = 0; i < n; i++) batch.push_back(buf[idx[i]]);
        return batch;
    }

    int size() const { return (int)buf.size(); }

    void write(std::ofstream& f, int state_size) const {
        int32_t n = (int32_t)buf.size();
        f.write(reinterpret_cast<const char*>(&n), sizeof(n));
        for (const auto& e : buf) {
            f.write(reinterpret_cast<const char*>(e.state.data()),
                    state_size * sizeof(float));
            f.write(reinterpret_cast<const char*>(e.next_state.data()),
                    state_size * sizeof(float));
            int32_t a = e.action;
            int8_t  d = e.done ? 1 : 0;
            f.write(reinterpret_cast<const char*>(&a),       sizeof(a));
            f.write(reinterpret_cast<const char*>(&e.reward), sizeof(float));
            f.write(reinterpret_cast<const char*>(&d),       sizeof(d));
        }
    }

    void read(std::ifstream& f, int state_size) {
        buf.clear();
        int32_t n = 0;
        f.read(reinterpret_cast<char*>(&n), sizeof(n));
        for (int32_t i = 0; i < n; i++) {
            Experience e;
            e.state.resize(state_size);
            e.next_state.resize(state_size);
            f.read(reinterpret_cast<char*>(e.state.data()),      state_size * sizeof(float));
            f.read(reinterpret_cast<char*>(e.next_state.data()), state_size * sizeof(float));
            int32_t a = 0; int8_t d = 0;
            f.read(reinterpret_cast<char*>(&a),       sizeof(a));
            f.read(reinterpret_cast<char*>(&e.reward), sizeof(float));
            f.read(reinterpret_cast<char*>(&d),       sizeof(d));
            e.action = a;
            e.done   = (d != 0);
            buf.push_back(std::move(e));
        }
    }

private:
    int capacity;
    std::deque<Experience> buf;
    std::mt19937 rng;
};

// ─── DQN Agent ────────────────────────────────────────────────────────────────

class DQNAgent {
public:
    NeuralNetwork policy_net, target_net;
    ReplayBuffer  buffer;

    float epsilon      = 1.0f;
    float epsilon_min  = 0.01f;
    float epsilon_decay = 0.995f;
    float gamma        = 0.95f;
    int   batch_size   = 64;
    int   target_sync_freq = 100;
    int   steps        = 0;

    DQNAgent(int state_size, int action_size, int hidden = 64)
        : policy_net(state_size, hidden, hidden, action_size, 0.001f),
          target_net(state_size, hidden, hidden, action_size, 0.001f),
          buffer(10000),
          action_size(action_size),
          rng(std::random_device{}())
    {
        target_net.copy_weights_from(policy_net);
    }

    int select_action(const std::vector<float>& state) {
        if (std::uniform_real_distribution<float>(0,1)(rng) < epsilon)
            return std::uniform_int_distribution<int>(0, action_size-1)(rng);
        auto q = policy_net.predict(state);
        return (int)(std::max_element(q.begin(), q.end()) - q.begin());
    }

    void store(std::vector<float> s, int a, float r, std::vector<float> s2, bool done) {
        buffer.push({std::move(s), std::move(s2), a, r, done});
    }

    // Weights only — for AI Play
    void save(const std::string& path) const { policy_net.save(path); }
    bool load(const std::string& path)       { return policy_net.load(path); }

    // Full checkpoint — for resuming training
    static constexpr uint32_t MAGIC = 0xDEAD1234;

    void save_checkpoint(const std::string& path, int episode) const {
        std::ofstream f(path, std::ios::binary);
        uint32_t magic = MAGIC;
        f.write(reinterpret_cast<const char*>(&magic),   sizeof(magic));
        f.write(reinterpret_cast<const char*>(&episode), sizeof(episode));
        f.write(reinterpret_cast<const char*>(&steps),   sizeof(steps));
        f.write(reinterpret_cast<const char*>(&epsilon), sizeof(epsilon));
        policy_net.write_full(f);
        target_net.write_full(f);
        buffer.write(f, policy_net.in_size);
    }

    bool load_checkpoint(const std::string& path, int& episode) {
        std::ifstream f(path, std::ios::binary);
        if (!f) return false;
        uint32_t magic = 0;
        f.read(reinterpret_cast<char*>(&magic), sizeof(magic));
        if (magic != MAGIC) return false;
        f.read(reinterpret_cast<char*>(&episode), sizeof(episode));
        f.read(reinterpret_cast<char*>(&steps),   sizeof(steps));
        f.read(reinterpret_cast<char*>(&epsilon), sizeof(epsilon));
        policy_net.read_full(f);
        target_net.read_full(f);
        buffer.read(f, policy_net.in_size);
        return f.good();
    }

    void train_step() {
        if (buffer.size() < batch_size) return;

        auto batch = buffer.sample(batch_size);
        int B = batch_size;
        int S = policy_net.in_size;

        Matrix x(B, S);
        std::vector<int>   actions(B);
        std::vector<float> targets(B);

        for (int i = 0; i < B; i++) {
            for (int j = 0; j < S; j++)
                x.at(i,j) = batch[i].state[j];
            actions[i] = batch[i].action;
            if (batch[i].done) {
                targets[i] = batch[i].reward;
            } else {
                auto q_next = target_net.predict(batch[i].next_state);
                targets[i]  = batch[i].reward
                            + gamma * *std::max_element(q_next.begin(), q_next.end());
            }
        }

        policy_net.train_batch(x, actions, targets);

        steps++;
        if (steps % target_sync_freq == 0)
            target_net.copy_weights_from(policy_net);

        if (epsilon > epsilon_min)
            epsilon *= epsilon_decay;
    }

private:
    int action_size;
    std::mt19937 rng;
};

// ─── State normalization ──────────────────────────────────────────────────────

inline std::vector<float> normalize_state(const std::array<int,8>& s) {
    // s[0..3]: pixel coords (0–800), s[4..7]: binary danger flags
    return {
        s[0] / 800.0f,
        s[1] / 800.0f,
        s[2] / 800.0f,
        s[3] / 800.0f,
        (float)s[4],
        (float)s[5],
        (float)s[6],
        (float)s[7],
    };
}

} // namespace dqn
