#include <bits/stdc++.h>

using namespace std;

int get_cell_id(int x, int y, int m) { return x * m + y + 1; }

vector<vector<int>> get_combinations(const vector<int> &elements, int k) {
  vector<vector<int>> combinations;

  int n = elements.size();

  if (k <= 0 || k > n)
    return combinations;

  int total = 1 << n;

  for (int i = 0; i < total; ++i) {
    if (__builtin_popcount(i) == k) {
      vector<int> comb;
      for (int j = 0; j < n; ++j) {
        if (i & (1 << j)) {
          comb.push_back(elements[j]);
        }
      }
      combinations.push_back(comb);
    }
  }
  return combinations;
}

void insert_cells(int x, int y, const vector<vector<int>> &matrix,
                  vector<vector<int>> &clauses) {
  const int vx[] = {1, 1, 1, 0, 0, -1, -1, -1};
  const int vy[] = {1, 0, -1, 1, -1, 1, 0, -1};
  const int n = matrix.size(), m = matrix[0].size();
  vector<int> elements;

  for (int i = 0; i < 8; i++) {
    int tx = x + vx[i], ty = y + vy[i];

    if (tx >= 0 && ty >= 0 && tx < n && ty < m && matrix[tx][ty] == -1) {
      elements.push_back(get_cell_id(tx, ty, m));
    }
  }

  int N = matrix[x][y];
  int m_neighbors = elements.size();

  if (N > m_neighbors) {
    clauses.push_back({});
    return;
  }

  int size_at_least = m_neighbors - N + 1;
  if (size_at_least > 0 && size_at_least <= m_neighbors) {
    vector<vector<int>> pos_combos = get_combinations(elements, size_at_least);
    for (const auto &comb : pos_combos) {
      clauses.push_back(comb);
    }
  }

  int size_at_most = N + 1;
  if (size_at_most > 0 && size_at_most <= m_neighbors) {
    vector<vector<int>> neg_combos = get_combinations(elements, size_at_most);
    for (const auto &comb : neg_combos) {
      vector<int> neg_clause;
      for (int var : comb) {
        neg_clause.push_back(-var);
      }
      clauses.push_back(neg_clause);
    }
  }
}

int main() {
  int n, m;
  cin >> n;
  cin >> m;

  vector<vector<int>> matrix(n, vector<int>(m, -1));

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cin >> matrix[i][j];
    }
  }

  vector<vector<int>> clauses;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (matrix[i][j] >= 0) {
        int id = get_cell_id(i, j, m);

        clauses.push_back({-id});

        insert_cells(i, j, matrix, clauses);
      }
    }
  }

  int total_vars = n * m;

  cout << "p cnf " << total_vars << " " << clauses.size() << '\n';

  for (const auto &clause : clauses) {
    for (int lit : clause) {
      cout << lit << " ";
    }
    cout << "0\n";
  }
}
