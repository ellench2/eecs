"""
EECS 445 - Introduction to Machine Learning
Winter 2023 - Project 2
Challenge
    Constructs a pytorch model for a convolutional neural network
    Usage: from model.challenge import Challenge
"""
import torch
import torch.nn as nn
import torch.nn.functional as F
from math import sqrt
from utils import config

class Challenge(nn.Module):
    def __init__(self):
        """Define the architecture, i.e. what layers our network contains. 
        At the end of __init__() we call init_weights() to initialize all model parameters (weights and biases)
        in all layers to desired distributions."""
        super().__init__()

        ## TODO: define each layer of your network
        self.conv1 = nn.Conv2d(3, 16, (5,5), (2,2), 1, bias=True)
        self.pool = nn.MaxPool2d((2,2), (2,2))
        self.conv2 = nn.Conv2d(16, 64, (5,5), (2,2), 1, bias=True)
        self.fc_ = nn.Linear(3136, 2, bias=True)
        ##

        self.init_weights()

    def init_weights(self):
        """Initialize all model parameters (weights and biases) in all layers to desired distributions"""
        ## TODO: initialize the parameters for your network
        torch.manual_seed(42)

        for conv in [self.conv1, self.conv2]:
            C_in = conv.weight.size(1)
            nn.init.normal_(conv.weight, 0.0, 1 / sqrt(5 * 5 * C_in))
            nn.init.constant_(conv.bias, 0.0)
        nn.init.normal_(self.fc_.weight, 1 / sqrt((self.fc_.weight.size(1))))
        nn.init.constant_(self.fc_.bias,0)
        ##

    def forward(self, x):
        """This function defines the forward propagation for a batch of input examples, by
            successively passing output of the previous layer as the input into the next layer (after applying
            activation functions), and returning the final output as a torch.Tensor object

            You may optionally use the x.shape variables below to resize/view the size of
            the input matrix at different points of the forward pass"""
        N, C, H, W = x.shape

        ## TODO: forward pass
        fout = nn.functional.relu(self.conv1(x))
        z = self.fc_(torch.flatten(nn.functional.relu(self.conv2(self.pool(fout))), 1, 3))
        ##

        return z
