#include "Node.h"

Node::Node() {
    printf("incomplete Node constructor\n");
    exit(1);
}

Node::Node(int _nID, int _me, State *_s, int _depth, bool isGoal) {
    int i, j;
    
    me = _me;
    s = _s;
    nID = _nID;

    numNext = s->numJointActions;
    depth = _depth;
    
    sigma_t = new double*[2];
    regret = new double*[2];
    for (j = 0; j < 2; j++) {
        sigma_t[j] = new double[s->numActions[j]];
        regret[j] = new double[s->numActions[j]];
        for (i = 0; i < s->numActions[j]; i++) {
            sigma_t[j][i] = 1.0 / s->numActions[j];
            regret[j][i] = 0.0;
        }
    }
    
    cuentita = new double[s->numActions[1-me]];
    
    reachability[0] = reachability[1] = 0.0;
    utility[0] = utility[1] = -100.0;
    duplo = 1;
    
    if (isGoal) {
        numNext = 0;
        utility[0] = utility[1] = 0.0;
    }
    
    visited[0] = visited[1] = false;
}

Node::~Node() {
    int j;
    for (j = 0; j < 2; j++) {
        delete sigma_t[j];
        delete regret[j];
    }
    delete sigma_t;
    delete regret;
    delete cuentita;
}

// assumes that the utilities of nodes further in the tree have already been computed
double Node::computeUtility(int index) {
    int i, j;
    
    utility[index] = 0.0;
    int jact;
    for (i = 0; i < s->numActions[0]; i++) {
        for (j = 0; j < s->numActions[1]; j++) {
            jact = s->jointCode(i, j);
            //if ((s->ID == 867) && (index == 1)) {
            //    printf("reward (%i, %i): (%lf, %lf) -> state %i to %i\n", i, j, s->rewards[jact][0][0], s->rewards[jact][0][1], s->ID, next[jact]->s->ID);
            //}
            utility[index] += sigma_t[0][i] * sigma_t[1][j] * (s->rewards[jact][0][index] + next[jact]->utility[index]);
        }
    }
    
    return utility[index];
}

void Node::updateRegret(int index) {
    if (numNext == 0)
        return;

    //if (s->ID == 827) {
    //    printf("reachability: %lf ... ", reachability[index]); fflush(stdout);
    //}
    
    double u[20];
    int i, j, jact;
    for (i = 0; i < s->numActions[index]; i++) {
        u[i] = 0.0;
        for (j = 0; j < s->numActions[1-index]; j++) {
            if (index == 0)
                jact = s->jointCode(i, j);
            else
                jact = s->jointCode(j, i);
            u[i] += sigma_t[1-index][j] * (s->rewards[jact][0][index] + next[jact]->utility[index]);
        }
        //if ((s->ID == 867) && (index == 1)) {
        //    printf("%i: u[i] = %.3lf; utility = %.3lf\n", i, u[i], utility[index]);
        //}
        
        regret[index][i] += reachability[index] * (u[i] - utility[index]);
    }
    
    //if (s->ID == 827) {
    //    printf("out\n");
    //}
}

void Node::updatePolicy(int index, int T) {
    if (numNext == 0) {
        return;
    }
    
    double rplus[20];
    double sum = 0.0;
    int i;
    for (i = 0; i < s->numActions[index]; i++) {
        rplus[i] = regret[index][i] / T;
        if (rplus[i] < 0)
            rplus[i] = 0.0;
        sum += rplus[i];
    }
    
    if (sum > 0.0) {
        double beta = 0.9;
        for (i = 0; i < s->numActions[index]; i++) {
            sigma_t[index][i] = rplus[i] / sum;
            //sigma_t[index][i] = beta * (rplus[i] / sum) + (1.0-beta) * (1.0 / s->numActions[index]);
        }
    }
    else {
        for (i = 0; i < s->numActions[index]; i++) {
            sigma_t[index][i] = 1.0 / s->numActions[index];
        }
    }
}

int Node::mfactorial(int choices) {
    int i;
    int val = 1;
    for (i = 2; i <= choices; i++)
        val *= i;
    
    return val;
}

void Node::print(int index, int T) {
    printf("reachability: %lf\n", reachability[index]);
    int i;
    printf("Regrets at node %i at time %i: ", s->ID, T);
    for (i = 0; i < s->numActions[index]; i++) {
        printf("%.3lf ", regret[index][i] / T);
    }
    printf("\n");
    printf("Policy at node %i at time %i: ", s->ID, T+1);
    for (i = 0; i < s->numActions[index]; i++) {
        printf("%.3lf ", sigma_t[index][i]);
    }
    printf("\n");
}

void Node::initCounts() {
    int j;
    
    for (j = 0; j < s->numActions[1-me]; j++) {
        cuentita[j] = sigma_t[1-me][j] * 5.0;
    }
}

void Node::updateSigmaNot() {
    double sum = 0.0;
    int j;
    for (j = 0; j < s->numActions[1-me]; j++) {
        sum += cuentita[j];
    }
    for (j = 0; j < s->numActions[1-me]; j++) {
        sigma_t[1-me][j] = cuentita[j] / sum;
    }
}


