(def input-text (slurp "src/advent/1/input.txt"))

(def input-vec (map #(Integer/parseInt %)
               (clojure.string/split input-text #"\n")))

;; part A
(reduce + input-vec)

;; part B
(defn first-freq [sequence]
  (def count-seq (count sequence))
  (defn looper [seen-set sum idx]
    (let [x (nth sequence idx)
          idx-nxt (mod (inc idx) count-seq)
          sum-nxt (+ sum x)]
      (if (contains? seen-set sum-nxt)
        sum-nxt
        (recur (conj seen-set sum-nxt)
               sum-nxt
               idx-nxt))))
  (looper #{0} 0 0))

(first-freq [0])
(first-freq [1 -1])
(first-freq [-6 3 8 5 -6])

(first-freq input-vec)                  ;; part B ans
