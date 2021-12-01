(defn zip [& colls]
  (partition (count colls) (apply interleave colls)))

(defn has-peak [pair]
  (< 0 (reduce - pair)))

(defn count-identity [coll]
  (count (filter identity coll)))

(defn count-peaks [coll dist]
  (count-identity (map has-peak
                       (zip (drop dist coll)
                            coll))))
(def v
  (map #(Integer/parseInt %)
       (clojure.string/split-lines (slurp "input"))))

(println (map #(count-peaks v %) '(1 3)))
