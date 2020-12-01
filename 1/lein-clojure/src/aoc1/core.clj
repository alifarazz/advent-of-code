(ns aoc1.core)

(defn f2sum [data]
  (first
   (filter (complement nil?)
           (for [i data
                 j data :while (<= i j)]
             (if (= (+ i j) 2020)
               (* i j))))))

(defn f3sum [data]
  (let [set-of-data (set data)]
    (first
     (filter (complement nil?)
             (for [i data
                   j data :while (<= i j)]
               (let [k (- 2020 (+ i j))]
                 (if (contains? set-of-data k)
                   (* i j k))))))))

(defn -main
  "AOC 1st day"
  [& args]
  (let [data (->> "../input"
                  (slurp)
                  (#(clojure.string/split % #"\n"))
                  (map #(Integer/parseInt %)))]
    (println (time (f2sum data)))
    (println (time (f3sum data)))))
