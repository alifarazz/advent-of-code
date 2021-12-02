(defn has-peak [pair]
  (< 0 (reduce - pair)))

(defn count-identity [coll]
  (count (filter identity coll)))

(defn count-peaks [coll dist]
  (->> (map vector (drop dist coll) coll)
       (map has-peak)
       count-identity))

(def v
  (map #(Integer/parseInt %)
       (clojure.string/split-lines (slurp "input"))))

(println (map #(count-peaks v %) '(1 3)))
