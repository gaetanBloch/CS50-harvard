CREATE TABLE transactions
  (id       INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
  user_id   INTEGER NOT NULL,
  timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
  type      TEXT CHECK(type IN ('P', 'S')),
  price     NUMERIC CHECK(price > 0) NOT NULL,
  shares    INTEGER CHECK(shares > 0) NOT NULL,
  symbol    TEXT CHECK(LENGTH(symbol) < 50) NOT NULL,
            FOREIGN KEY(user_id) REFERENCES users(id));

CREATE TABLE shares
  (user_id  INTEGER NOT NULL,
  symbol    TEXT CHECK(LENGTH(symbol) < 50) NOT NULL,
  count     INTEGER CHECK(count > 0) NOT NULL,
            PRIMARY KEY(user_id, symbol),
            FOREIGN KEY(user_id) REFERENCES users(id));

CREATE UNIQUE INDEX timestamp ON transactions(timestamp);
CREATE INDEX t_symbol ON transactions(symbol);
CREATE INDEX s_symbol ON shares(symbol);
CREATE UNIQUE INDEX user_symbol ON shares(user_id, symbol);