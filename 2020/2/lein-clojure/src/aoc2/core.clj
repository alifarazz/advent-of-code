(ns aoc2.core)

(defn xor [a b] (or (and (not a) b) (and a (not b))))

(defn parse-input [entry]
  (defn parse-entry [bounds letter password]
    (let [a (-> bounds
                (first)
                (Integer/parseInt))
          b (-> bounds
                (fnext)
                (Integer/parseInt))
          letter (first letter)]
      (list a b letter password)))
  (let [entry-split (clojure.string/split entry #" ")]
    (parse-entry (clojure.string/split (first entry-split) #"-")
                 (fnext entry-split)
                 (nth entry-split 2))))

(defn password-valid-one? [entry]
  (let [a (first entry)
        b (nth entry 1)
        letter (nth entry 2)
        password (nth entry 3)
        cnt (count (filter #(= letter %) password))]
    (and (<= a cnt) (<= cnt b))))

(defn password-valid-two? [entry]
  (let [a (dec (first entry))
        b (dec (nth entry 1))
        letter (nth entry 2)
        password (nth entry 3)]
    (xor (= (nth password a) letter)
         (= (nth password b) letter))))

(defn -main
  "AOC 2nd day"
  [& args]
  (let [data (-> "../input"
                 (slurp)
                 (clojure.string/split #"\n")
                 (#(map parse-input %)))]
    (println (time (count (filter password-valid-one? data))))
    (println (time (count (filter password-valid-two? data))))))
