import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim
import random


class DriveNet(nn.Module):
    def __init__(self):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(5, 32),
            nn.ReLU(),
            nn.Linear(32, 32),
            nn.ReLU(),
            nn.Linear(32, 16),
            nn.ReLU(),
            nn.Linear(16, 2),
            nn.Sigmoid(),
        )

    def forward(self, x):
        return self.net(x) * 100.0


def predict(model, x_self, y_self, angle_self, x_target, y_target):
    model.eval()
    inp = torch.tensor([[x_self, y_self, angle_self, x_target, y_target]], dtype=torch.float32)
    with torch.no_grad():
        out = model(inp)
    return out[0, 0].item(), out[0, 1].item()


def train_simple():
    model = DriveNet()
    optimizer = optim.Adam(model.parameters(), lr=0.001)
    criterion = nn.MSELoss()

    data = generate_training_data()
    X = torch.tensor(data["X"], dtype=torch.float32)
    y = torch.tensor(data["y"], dtype=torch.float32)

    for epoch in range(300):
        model.train()
        optimizer.zero_grad()
        pred = model(X)
        loss = criterion(pred, y)
        loss.backward()
        optimizer.step()
        if (epoch + 1) % 50 == 0:
            print(f"Epoch {epoch+1}, Loss: {loss.item():.6f}")

    return model


def generate_training_data():
    X, y = [], []
    for _ in range(10000):
        x_self = random.uniform(-5.0, 5.0)
        y_self = random.uniform(-5.0, 5.0)
        angle_self = random.uniform(-np.pi, np.pi)
        x_target = random.uniform(-5.0, 5.0)
        y_target = random.uniform(-5.0, 5.0)

        dx = x_target - x_self
        dy = y_target - y_self
        dist = np.sqrt(dx * dx + dy * dy)

        target_angle = np.arctan2(dy, dx)
        angle_diff = (target_angle - angle_self + np.pi) % (2 * np.pi) - np.pi

        if abs(angle_diff) > 0.5:
            factor = 1.0 if angle_diff > 0 else -1.0
            left = 40.0 + 30.0 * factor
            right = 40.0 - 30.0 * factor
        elif dist > 1.0:
            speed = min(100.0, 30.0 + dist * 15.0)
            correction = angle_diff * 20.0
            left = speed - correction
            right = speed + correction
        elif dist > 0.2:
            left = 20.0
            right = 20.0
        else:
            left = 0.0
            right = 0.0

        X.append([x_self, y_self, angle_self, x_target, y_target])
        y.append([max(0.0, min(100.0, left)), max(0.0, min(100.0, right))])

    return {"X": X, "y": y}


if __name__ == "__main__":
    model = train_simple()
    torch.save(model.state_dict(), "drive_net.pth")
    print("Model saved to drive_net.pth")
